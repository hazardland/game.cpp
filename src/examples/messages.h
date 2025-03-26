#pragma once

#pragma pack(push, 1)
struct FootmanState {
    static constexpr uint8_t type = 1;
    uint32_t object_id;
    float x;
    float y;
    uint32_t mode;
    uint32_t modeX;
    uint32_t modeY;
};
#pragma pack(pop)