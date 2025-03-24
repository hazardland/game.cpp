// #include <libwebsockets.h>
// #include <string.h>
// #include <iostream>

// static int callback_ws(struct lws *wsi, enum lws_callback_reasons reason,
//                        void *user, void *in, size_t len) {
//     switch (reason) {
//         case LWS_CALLBACK_CLIENT_ESTABLISHED:
//             std::cout << "[Connected]\n";
//             lws_callback_on_writable(wsi);
//             break;

//         case LWS_CALLBACK_CLIENT_RECEIVE:
//             std::cout << "[Message] " << std::string((char*)in, len) << "\n";
//             break;

//         case LWS_CALLBACK_CLIENT_WRITEABLE: {
//             const char* msg = "{\"type\":\"hello\"}";
//             unsigned char buf[LWS_PRE + 512];
//             size_t msg_len = strlen(msg);
//             memcpy(&buf[LWS_PRE], msg, msg_len);
//             lws_write(wsi, &buf[LWS_PRE], msg_len, LWS_WRITE_TEXT);
//             break;
//         }

//         case LWS_CALLBACK_CLOSED:
//             std::cout << "[Disconnected]\n";
//             break;

//         default:
//             break;
//     }
//     return 0;
// }

// static struct lws_protocols protocols[] = {
//     { "my-protocol", callback_ws, 0, 4096 },
//     { NULL, NULL, 0, 0 } // terminator
// };

// int main() {
//     struct lws_context_creation_info info;
//     memset(&info, 0, sizeof(info));

//     info.port = CONTEXT_PORT_NO_LISTEN;
//     info.protocols = protocols;
//     info.options |= LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;

//     struct lws_context *context = lws_create_context(&info);

//     struct lws_client_connect_info ccinfo = {};
//     ccinfo.context = context;
//     ccinfo.address = "echo.websocket.events";
//     ccinfo.port = 443;
//     ccinfo.path = "/";
//     ccinfo.host = ccinfo.address;
//     ccinfo.origin = ccinfo.address;
//     ccinfo.ssl_connection = LCCSCF_USE_SSL;
//     ccinfo.protocol = "my-protocol";

//     lws_client_connect_via_info(&ccinfo);

//     while (lws_service(context, 100) >= 0);

//     lws_context_destroy(context);
//     return 0;
// }

// /// g++ -std=c++20 -O2 ws_client.cpp -o ws_client -lwebsockets -lssl -lcrypto -lz -lpthread -lws2_32

// File: ws_client.cpp

#include <libwebsockets.h>
#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include <functional>
#include <cstring>

class uClient {
public:
    using OnMessageCallback = std::function<void(const std::string&)>;

    uClient() {}
    ~uClient() { close(); }

    bool connect(const std::string& uri) {
        host = "echo.websocket.events";  // hardcoded for now
        path = "/";
        port = 443;

        static struct lws_protocols protocols[] = {
            { "my-protocol", &uClient::callback, 0, 4096, 0, this, 0 },
            { NULL, NULL, 0, 0, 0, NULL, 0 }
        };

        struct lws_context_creation_info info;
        memset(&info, 0, sizeof(info));
        info.port = CONTEXT_PORT_NO_LISTEN;
        info.protocols = protocols;
        info.options = LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;

        context = lws_create_context(&info);
        if (!context) return false;

        struct lws_client_connect_info ccinfo = {};
        ccinfo.context = context;
        ccinfo.address = host.c_str();
        ccinfo.port = port;
        ccinfo.path = path.c_str();
        ccinfo.host = host.c_str();
        ccinfo.origin = host.c_str();
        ccinfo.ssl_connection = LCCSCF_USE_SSL;
        ccinfo.protocol = "my-protocol";
        ccinfo.userdata = this;

        wsi = lws_client_connect_via_info(&ccinfo);
        if (!wsi) return false;

        running = true;
        clientThread = std::thread([this]() {
            while (running) {
                lws_service(context, 100);
            }
        });

        return true;
    }

    void send(const std::string& message) {
        sendBuffer = message;
        wantToSend = true;
        if (wsi) lws_callback_on_writable(wsi);
    }

    void close() {
        running = false;
        if (clientThread.joinable()) {
            clientThread.join();
        }
        if (context) {
            lws_context_destroy(context);
            context = nullptr;
        }
    }

    void setOnMessage(OnMessageCallback callback) {
        onMessage = callback;
    }

private:
    static int callback(struct lws* wsi, enum lws_callback_reasons reason,
                        void* user, void* in, size_t len) {
        uClient* self = reinterpret_cast<uClient*>(user);

        switch (reason) {
            case LWS_CALLBACK_CLIENT_ESTABLISHED:
                std::cout << "[uClient] Connected\n";
                break;

            case LWS_CALLBACK_CLIENT_RECEIVE:
                if (self && self->onMessage) {
                    self->onMessage(std::string((const char*)in, len));
                }
                break;

            case LWS_CALLBACK_CLIENT_WRITEABLE:
                if (self && self->wantToSend) {
                    unsigned char buf[LWS_PRE + 1024];
                    std::memcpy(&buf[LWS_PRE], self->sendBuffer.c_str(), self->sendBuffer.size());
                    lws_write(wsi, &buf[LWS_PRE], self->sendBuffer.size(), LWS_WRITE_TEXT);
                    self->wantToSend = false;
                }
                break;

            case LWS_CALLBACK_CLOSED:
            case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
                std::cout << "[uClient] Disconnected\n";
                break;

            default:
                break;
        }

        return 0;
    }

    std::thread clientThread;
    std::atomic<bool> running{false};
    struct lws_context* context = nullptr;
    struct lws* wsi = nullptr;
    OnMessageCallback onMessage;

    std::string sendBuffer;
    std::atomic<bool> wantToSend{false};
    std::string host;
    std::string path;
    int port = 443;
};

// -----------------------------
// 🧪 Test it
int main() {
    uClient client;
    client.setOnMessage([](const std::string& msg) {
        std::cout << "[Server] " << msg << "\n";
    });

    if (client.connect("wss://echo.websocket.events")) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        client.send("Hello from one-file client!");
        std::this_thread::sleep_for(std::chrono::seconds(5));
        client.close();
    } else {
        std::cerr << "Failed to connect.\n";
    }

    return 0;
}
