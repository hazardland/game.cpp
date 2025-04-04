// File: window.h

#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "game/cooldown.h"

class State;
class Scene;

extern bool SDL_STARTED;  // Declare the global variable; it will be defined in the .cpp file.

/**
 * @brief The Window class extends from the Screen class and represents a window in the game.
 *
 * This class manages an SDL_Window object, and provides methods for rendering and updating scenes, resizing the window, and running the game loop.
 */
class Window {

private:

    SDL_Window* window;
    Scene* scene;
    State* state;
    bool fullscreen = false;  // Track fullscreen state
    Cooldown* fullscreenCooldown;
    int originalWidth;
    int originalHeight;

public:

    Window(const char* title, const int width, const int height, State* state = NULL);
    void setScene(Scene* scene);
    void onResize(int width, int height); //override;
    SDL_Window* getInstance();
    void toggleFullscreen();  // New method
    bool isFullscreen();
    int run();
};

#endif // GAME_WINDOW_H
