/**
 * @file
 * @brief Contains the definition of the Camera class
 */

#ifndef GAME_CAMERA_H
#define GAME_CAMERA_H

#include <SDL2/SDL.h>

/**
 * @class Camera
 * @brief A class that handles camera tracking in the game.
 *
 * This class handles the view of the game world, including the visibility and positioning
 * of game objects. It is able to update these based on the game state.
 */
class Camera {
    public:

    SDL_FRect result;  ///< The rectangle representing the resulting camera view after translation
    int x=0;           ///< The x-coordinate of the camera
    int y=0;           ///< The y-coordinate of the camera
    int width;         ///< The width of the camera view
    int height;        ///< The height of the camera view

    /**
     * @brief Sets the size of the camera view.
     * @param width The width of the camera view.
     * @param height The height of the camera view.
     */
    void setSize(int width, int height);

    /**
     * @brief Checks whether a game object is visible in the camera view.
     * @param position The position of the game object.
     * @return true if the game object is visible, false otherwise.
     */
    bool isVisible(SDL_FRect* position);

    /**
     * @brief Translates the position of a game object based on the camera view.
     * @param position The position of the game object.
     * @return A pointer to the resulting rectangle after translation.
     */
    SDL_FRect* translate(SDL_FRect* position);
};

#endif
