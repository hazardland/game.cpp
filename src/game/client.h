#pragma once

#include <libwebsockets.h>

#include <string>
#include <functional>
#include <vector>

class Client {
public:
    using OnMessageCallback = std::function<void(const std::string&)>;

    Client();
    ~Client();

    bool connect(const std::string& uri);   // wss://host[:port]/path
    void poll();                            // Call this every frame
    void send(const std::string& message);  // Queue message for sending
    void setOnMessage(OnMessageCallback cb);

    bool isConnected();
    void close();
    void enableAutoReconnect(bool enable);
    // void setMaxQueueSize(size_t maxSize);
    
private:
    static int callback(struct lws* wsi, enum lws_callback_reasons reason,
        void* user, void* in, size_t len);
        
        lws_context* context = nullptr;
        lws* wsi = nullptr;
        std::vector<std::string> outgoing;
        OnMessageCallback onMessage;
        bool connected = false;
        bool autoReconnect = true;
        // size_t maxQueueSize = 100;
        std::string reconnectUri;
        
    static constexpr int MAX_PAYLOAD = 4096;
};
