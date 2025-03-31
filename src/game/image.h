#ifndef GAME_IMAGE_H
#define GAME_IMAGE_H

#include <SDL3_image/SDL_image.h>

/**
 * @class Image
 * @brief Class for loading, managing, and rendering textures.
 *
 * The Image class prepares textures for loading into GPU RAM.
 * It renders a region of an image at a provided position.
 * It can also replace colors in the source image before transferring to GPU RAM.
 */
class Image {
public:
    /// Constructor that takes a renderer and a path to an image file.
    Image(SDL_Renderer* renderer, const char* path);

    /// Destructor that cleans up the SDL_Texture.
    ~Image();

    /**
     * @brief Retrieve the width of the image.
     * @return The width of the image.
     */
    int getWidth();

    /**
     * @brief Retrieve the height of the image.
     * @return The height of the image.
     */
    int getHeight();

    /**
     * @brief Render the image.
     * @param frame The portion of the texture to render.
     * @param position The location at which to render the texture.
     * @param flip The SDL_FlipMode value to apply to the texture.
     */
    void render(SDL_FRect* frame, SDL_FRect* position, SDL_FlipMode flip = SDL_FLIP_NONE);

    SDL_Renderer* renderer;    ///< Pointer to the SDL_Renderer.
private:
    SDL_Texture* texture;      ///< Pointer to the SDL_Texture.
    float width;                 ///< Width of the image.
    float height;                ///< Height of the image.
};

#endif // GAME_IMAGE_H
