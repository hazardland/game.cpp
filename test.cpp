// test3.cpp
#include <iostream>
#include <chrono>

class Cooldown {
public:
    Cooldown(int cooldownMs)
        : cooldownTime(std::chrono::milliseconds(cooldownMs)),
          lastActionTime(std::chrono::steady_clock::now()) {}

    bool isReady() {
        auto now = std::chrono::steady_clock::now();
        return now - lastActionTime >= cooldownTime;
    }

    void reset() {
        lastActionTime = std::chrono::steady_clock::now();
    }

private:
    std::chrono::milliseconds cooldownTime;
    std::chrono::steady_clock::time_point lastActionTime;
};

int main() {
    Cooldown c(1000);
    std::cout << (c.isReady() ? "Ready\n" : "Not ready\n");
    return 0;
}
