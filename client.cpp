// client.cpp
#include <game/client.h>
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    Client client;

    client.setOnMessage([](const std::string& msg) {
        std::cout << "[Server] " << msg << std::endl;
    });

    client.enableAutoReconnect(true);
    // client.setMaxQueueSize(50);

    if (!client.connect("ws://localhost:9000")) {
        return 1;
    }

    int i = 0;
    auto start = std::chrono::steady_clock::now();
    
    while (true) {
        client.poll();
        // std::this_thread::sleep_for(std::chrono::milliseconds(10));
    

        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
    
        if (true || elapsed.count() >= 5) { 
            std::string msg = "Message " + std::to_string(i++);
            client.send(msg);
            start = now;
        }
    }

    return 0;
}

// g++ client.cpp src/game/client.cpp -I./src -lwebsockets -o client
