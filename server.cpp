// server.cpp
#include <libwebsockets.h>

#include <cstring>
#include <iostream>

static int callback_echo(struct lws* wsi, enum lws_callback_reasons reason,
    void* user, void* in, size_t len)
    {
        switch (reason) {
            case LWS_CALLBACK_ESTABLISHED:
            std::cout << "[Server] Client connected.\n";
            break;
            
            case LWS_CALLBACK_RECEIVE:
            std::cout << "[Server] Received: " << std::string((char*)in, len) << "\n";
            // Echo it back
            lws_write(wsi, (unsigned char*)in, len, LWS_WRITE_TEXT);
            break;
            
            default:
            break;
        }
        return 0;
    }
    
int main() {
    // lws_set_log_level(LLL_ERR, nullptr);  // Only show errors
    lws_set_log_level(LLL_ERR | LLL_WARN | LLL_NOTICE | LLL_INFO, NULL);

    lws_protocols protocols[] = {
        { "ws", callback_echo, 0, 4096 }, // 👈 change "echo-protocol" to "ws"
        { nullptr, nullptr, 0, 0 }
    };

    lws_context_creation_info info = {};
    info.port = 9000;
    info.protocols = protocols;
    info.gid = -1;
    info.uid = -1;

    lws_context* context = lws_create_context(&info);
    if (!context) {
        std::cerr << "Failed to create server context\n";
        return 1;
    }

    std::cout << "[Server] Listening on ws://localhost:9000\n";

    while (true) {
        lws_service(context, 50);
    }

    lws_context_destroy(context);
    return 0;
}

// g++ server.cpp -o server -lwebsockets 