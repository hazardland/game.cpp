#ifndef GAME_FRAME_H
#define GAME_FRAME_H

#include <SDL2/SDL_image.h>

/**
 * @class Frame
 * @brief Defines a rectangle of the original texture to be used as a frame.
 *
 * The Frame class is useful for handling animation frames or any situation where only a portion
 * of a texture should be rendered. It also handles horizontal and vertical flipping of the texture.
 */
class Frame {
public:
    /// Default constructor
    Frame();

    /// Destructor
    ~Frame() = default;

    /**
     * @brief Retrieve the current SDL_RendererFlip value.
     * @return The current SDL_RendererFlip value.
     */
    SDL_RendererFlip getFlip();

    /**
     * @brief Cache the SDL_RendererFlip value based on the flipX and flipY boolean values.
     */
    void cacheFlip();

    /**
     * @brief Retrieve the SDL_Rect of the frame.
     * @return Pointer to the SDL_Rect of the frame.
     */
    SDL_Rect* getRect();

    SDL_Rect rect;   ///< Defines the x, y, width, and height of the rectangle.

    bool flipX = false;  ///< Boolean flag indicating if the frame should be flipped horizontally.

    bool flipY = false;  ///< Boolean flag indicating if the frame should be flipped vertically.

private:
    SDL_RendererFlip flip;  ///< SDL_RendererFlip value for flipping the texture.
};

#endif // GAME_FRAME_H
