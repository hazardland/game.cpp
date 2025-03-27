#include <libwebsockets.h>
#include <iostream>
#include <deque>
#include <unordered_map>
#include <algorithm>
#include <cstring>
#include <vector>

static constexpr int MAX_PAYLOAD = 4096;
static constexpr size_t MAX_QUEUE_SIZE = 10000;

struct ClientData {
    std::deque<std::vector<uint8_t>> outgoing;
};

static std::vector<lws*> clients;
static std::unordered_map<lws*, ClientData> clientDataMap;

static int callback_broadcast(struct lws* wsi, enum lws_callback_reasons reason,
                              void* user, void* in, size_t len) {
    switch (reason) {
        case LWS_CALLBACK_ESTABLISHED:
            std::cout << "[Server] Client connected.\n";
            clients.push_back(wsi);
            clientDataMap[wsi] = ClientData{};
            break;

        case LWS_CALLBACK_RECEIVE: {
            // Validate message size
            if (len > MAX_PAYLOAD) {
                std::cerr << "[Server] Dropping oversized message: " << len << " bytes\n";
                break;
            }

            std::vector<uint8_t> msg(reinterpret_cast<uint8_t*>(in),
                                     reinterpret_cast<uint8_t*>(in) + len);

            // Queue message for all clients except sender
            for (auto* client : clients) {
                if (client == wsi)
                    continue;

                auto& queue = clientDataMap[client].outgoing;

                // if (queue.size() > MAX_QUEUE_SIZE) {
                //     std::cerr << "[Server] Disconnecting client due to queue overflow\n";
                //     lws_set_timeout(client, PENDING_TIMEOUT_KILLED_BY_SERVER, LWS_TO_KILL_ASYNC);
                //     continue;
                // }
                // printf("Sending");
                queue.push_back(msg);  // message is small, so copy is okay
                lws_callback_on_writable(client);  // Mark for writing
            }
            break;
        }

        case LWS_CALLBACK_SERVER_WRITEABLE: {
            auto& queue = clientDataMap[wsi].outgoing;
            if (!queue.empty()) {
                const auto& msg = queue.front();

                if (msg.size() > MAX_PAYLOAD) {
                    std::cerr << "[Server] Skipping oversized queued message\n";
                    queue.pop_front();
                    break;
                }

                unsigned char buf[LWS_PRE + MAX_PAYLOAD];
                std::memcpy(&buf[LWS_PRE], msg.data(), msg.size());

                int bytes_written = lws_write(wsi, &buf[LWS_PRE], msg.size(), LWS_WRITE_BINARY);
                if (bytes_written < 0) {
                    std::cerr << "[Server] lws_write failed\n";
                    return -1;
                }

                queue.pop_front();

                if (!queue.empty()) {
                    lws_callback_on_writable(wsi);  // More messages to send
                }
            }
            break;
        }

        case LWS_CALLBACK_CLOSED:
        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
            std::cout << "[Server] Client disconnected.\n";
            clients.erase(std::remove(clients.begin(), clients.end(), wsi), clients.end());
            clientDataMap.erase(wsi);
            break;

        default:
            break;
    }

    return 0;
}

int main() {
    lws_set_log_level(LLL_ERR, nullptr);  // Only show errors
    // lws_set_log_level(LLL_ERR | LLL_WARN | LLL_NOTICE | LLL_INFO, nullptr);

    lws_protocols protocols[] = {
        { "ws", callback_broadcast, 0, MAX_PAYLOAD },
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
        lws_service(context, 0); // non-blocking
    }

    lws_context_destroy(context);
    return 0;
}

// g++ server.cpp -o server -lwebsockets