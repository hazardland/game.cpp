#ifndef GAME_TORCH_H
#define GAME_TORCH_H

#include "game/object.h"

class State;

class Torch : public Object {
    public:
        SDL_Color color;
        int coreRadius;
        int lightRadius;
        bool visible;
        SDL_Texture* texture = nullptr;

    
        Torch(SDL_Color color, int coreRadius = 96, int lightRadius = 320);
        void render(State* state) override;
        void update(State* state) override;
    };
    

#endif // GAME_TORCH_H
