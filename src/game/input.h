#ifndef GAME_INPUT_H
#define GAME_INPUT_H

struct Input {
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool space = false;
    void reset() {
        up = down = left = right = space = false;
    }
};

#endif
