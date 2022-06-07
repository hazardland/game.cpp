#ifndef GAME_INPUT_H
#define GAME_INPUT_H

class Keyboard {
    public:
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool space = false;
    const Uint8* keys;
    void reset() {
        up = down = left = right = space = false;
    }
};

class Mouse {
    public:
    
    int x;
    int y;

    bool rightDragActive = false;
    bool rightDragEnded = false;
    int rightDragStartX = -1;
    int rightDragStartY = -1;
    bool rightClick = false;
   
    bool leftDragActive = false;   
    bool leftDragEnded = false;
    int leftDragStartX = -1;
    int leftDragStartY = -1;
    bool leftClick = false;


    void reset() {
        rightClick = leftClick =  false;
    }

};

class Input {
    public:
    Keyboard* keyboard = new Keyboard();
    Mouse* mouse = new Mouse();
};

#endif
