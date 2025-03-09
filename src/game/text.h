// File: text.h

#ifndef GAME_TEXT_H
#define GAME_TEXT_H

#include <string>
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// #include "game/state.h"
#include "game/object.h"

// class Object;
class State;

/**
 * @brief The Text class extends from the Object class and represents a text object in the game.
 *
 * This class manages a texture that contains the rendered text, the font used, color of the text,
 * and provides methods for rendering and manipulating the text.
 */
class Text: public Object {
private:
    SDL_Texture* texture = NULL;
    TTF_Font* font;
    SDL_Color color;
    std::string text;
    // std::map<std::string, SDL_Texture*> cache;
    bool prepared = false;
    bool positionFixed = true;
public:
    Text(TTF_Font* font, std::string text = "", int x = 0, int y = 0, SDL_Color color = {255, 255, 255});
    Text(TTF_Font* font, Position* position, std::string text = "", SDL_Color color = {255, 255, 255});
    Text* setText(std::string text);
    Text* appendText(std::string text);
    Text* setColor(SDL_Color color);
    Text* setPositionFixed(bool value);
    virtual void render(State* state) override;
    ~Text();
};

#endif // GAME_TEXT_H
