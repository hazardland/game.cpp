#include "client.h"
#include <cstring>
#include <iostream>
#include <thread>


Client::Client() {
    // lws_set_log_level(LLL_ERR, nullptr);  // Only show errors
    lws_set_log_level(LLL_ERR | LLL_WARN | LLL_NOTICE | LLL_INFO, NULL);

}

Client::~Client() {
    if (context) {
        lws_context_destroy(context);
    }
}

bool Client::connect(const std::string& uri) {

    // The first part about uri extracting

    // Parse URI safely
    std::string address = "localhost";
    int port = 443;
    std::string path = "/";
    bool ssl = false;
    

    std::string uri_ = uri;
    if (uri_.rfind("wss://", 0) == 0) {
        ssl = true;
        uri_ = uri_.substr(6);
    } else if (uri_.rfind("ws://", 0) == 0) {
        uri_ = uri_.substr(5);
    }

    // Extract path
    auto slashPos = uri_.find('/');
    if (slashPos != std::string::npos) {
        path = uri_.substr(slashPos);
        uri_ = uri_.substr(0, slashPos);
    }
    
    // Extract host and port
    auto colonPos = uri_.find(':');
    if (colonPos != std::string::npos) {
        address = uri_.substr(0, colonPos);
        std::string portStr = uri_.substr(colonPos + 1);
        try {
            port = std::stoi(portStr);
        } catch (...) {
            std::cerr << "[Client] Invalid port: " << portStr << std::endl;
            return false;
        }
    } else {
        address = uri_;
        port = ssl ? 443 : 80;
    }
    
    printf("[Client] uri: %s --> ssl: %s, path: %s, address: %s, port: %d\n",
            uri.c_str(),   
            ssl ? "true" : "false",
            path.c_str(),
            address.c_str(),
            port);
    
    // We need to kind of define protocol
    // Just naming that if "ws" comes we should handle with Client:callback function
    // You do not see this in high level socket implenetations
    // And this works on node.js server also
    lws_protocols protocols[] = {
        { "ws", Client::callback, 0, MAX_PAYLOAD },
        { nullptr, nullptr, 0, 0 }
    };


    // Now context idk what is this also
    lws_context_creation_info ctxInfo = {};
    ctxInfo.port = CONTEXT_PORT_NO_LISTEN;
    ctxInfo.protocols = protocols;
    ctxInfo.gid = -1;
    ctxInfo.uid = -1;
    ctxInfo.ka_time = 15;      // After 15s of inactivity, start keepalive
    ctxInfo.ka_interval = 5;   // Send pings every 5s
    ctxInfo.ka_probes = 3;     // Allow 3 failed pings before dropping

    context = lws_create_context(&ctxInfo);
    if (!context) {
        std::cerr << "[Client] Failed to create lws context\n";
        return false;
    }
    
    // connectInfo makes lot of sense
    // It takes some params we parsed from uri
    lws_client_connect_info connectInfo = {};
    connectInfo.context = context;
    connectInfo.address = address.c_str();
    connectInfo.port = port;
    connectInfo.path = path.c_str();
    connectInfo.host = address.c_str();
    connectInfo.origin = "origin";
    connectInfo.ssl_connection = ssl ? LCCSCF_USE_SSL : 0;
    connectInfo.protocol = "ws";
    connectInfo.userdata = this;

    // I use this for reconnection
    reconnectUri = uri;

    // So finally we used info for connection
    wsi = lws_client_connect_via_info(&connectInfo);
    if (!wsi) {
        std::cerr << "[Client] Connection failed\n";
        return false;
    }

    // Probably this should be in separate thread?
    // poll just contains lws_service(context, 0);
    // So it is like "process socket things" type of a thing
    int retries = 0;
    while (!connected && retries++ < 500) {
        poll();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    if (!connected) {
        std::cerr << "[Client] Connection failed\n";
        return false;
    }

    // So TLDR we configured connection with 

    return true;
}

void Client::poll() {
    if (context) {
        lws_cancel_service(context);
        lws_service(context, 0);         // non-blocking
        // lws_service_tsi(context, 1, 0);  // 1 ms timeout

    }
}

void Client::send(const std::string& message) {
    if (!wsi || !connected) {
        if (!autoReconnect) {
            std::cerr << "[Client] No connection and autoReconnect disabled. Message dropped.\n";
            return;
        }

        std::cerr << "[Client] Disconnected. Attempting inline reconnect...\n";
        
        // Reconnect attempt (retries up to 5s)
        int retries = 0;
        while ((!wsi || !connected) && retries++ < 500) {
            poll();  // process reconnect logic
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        if (!wsi || !connected) {
            std::cerr << "[Client] Inline reconnect failed. Message dropped.\n";
            return;
        }

        std::cerr << "[Client] Reconnected. Proceeding to send message.\n";
    }

    // if (outgoing.size() >= maxQueueSize) {
    //     std::cerr << "[Client] Message queue full. Dropping message.\n";
    //     return;
    // }

    std::cout << "[Client] Queuing: " << message << std::endl;
    outgoing.push_back(message);
    lws_callback_on_writable(wsi);  // trigger LWS_CALLBACK_CLIENT_WRITEABLE

    // Optionally flush immediately
    while (!outgoing.empty()) {
        poll();  // or lws_service(context, 0);
    }
}


void Client::setOnMessage(OnMessageCallback cb) {
    onMessage = cb;
}

bool Client::isConnected()
{
    return connected;
}

int Client::callback(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* in, size_t len) {
    
    Client* self = reinterpret_cast<Client*>(lws_wsi_user(wsi));

    switch (reason) {

        case LWS_CALLBACK_CLIENT_ESTABLISHED:
            std::cout << "[Client] Connected\n";
            self->connected = true;
            
            if (!self->outgoing.empty()) {
                lws_callback_on_writable(wsi);
            }
            break;
            
            // lws_set_timer_usecs(wsi, 15 * LWS_USEC_PER_SEC);

        case LWS_CALLBACK_CLIENT_RECEIVE:
            if (self && self->onMessage) {
                self->onMessage(std::string((const char*)in, len));
            }
            break;

        // case LWS_CALLBACK_TIMER:
        //     std::cout << "[Client] Keep-alive timer triggered\n";
        
        //     // Send a ping (or any lightweight keepalive message)
        //     if (self && self->connected) {
        //         self->send("ping");
        //     }
        
        //     // Reset the timer for next keepalive
        //     lws_set_timer_usecs(wsi, 15 * LWS_USEC_PER_SEC);
        //     break;        

        case LWS_CALLBACK_CLIENT_WRITEABLE:
            if (self && !self->outgoing.empty()) {
                std::string msg = self->outgoing.front();

                unsigned char buf[LWS_PRE + MAX_PAYLOAD];
                std::memcpy(&buf[LWS_PRE], msg.c_str(), msg.size());

                std::cout << "[Client] Sending: " << msg << std::endl;
                int bytes_written = lws_write(wsi, &buf[LWS_PRE], msg.size(), LWS_WRITE_TEXT);

                if (bytes_written < 0) {
                    std::cerr << "[Client] lws_write failed! Will retry on next writeable.\n";
                    // Don't erase the message, try again on the next writeable
                    return -1;
                }

                // ✅ Only remove the message if it was successfully written
                self->outgoing.erase(self->outgoing.begin());

                if (!self->outgoing.empty()) {
                    lws_callback_on_writable(wsi);
                }
            }
            break;

        case LWS_CALLBACK_CLIENT_CLOSED:
        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
            std::cerr << "[Client] Connection error\n";
            if (in && len > 0)
                std::cerr << "Error: " << std::string((char*)in, len) << "\n";

            self->connected = false;

            // if (self->context) {
            //     lws_context_destroy(self->context);
            //     self->context = nullptr;
            // }
            self->wsi = nullptr;

            if (self->autoReconnect && !self->reconnectUri.empty()) {
                std::cerr << "[Client] Attempting reconnect...\n";
                self->connect(self->reconnectUri);
            }
            break;
        

    default:
        break;
    }

    return 0;
}

void Client::close() {
    if (wsi) {
        lws_set_timeout(wsi, PENDING_TIMEOUT_CLOSE_SEND, LWS_TO_KILL_ASYNC);
        wsi = nullptr;
    }

    if (context) {
        lws_context_destroy(context);
        context = nullptr;
    }

    connected = false;
    std::cout << "[Client] Connection closed\n";
}

void Client::enableAutoReconnect(bool enable) {
    autoReconnect = enable;
}

// void Client::setMaxQueueSize(size_t maxSize) {
//     maxQueueSize = maxSize;
// }