#include "examples/farm.h"

// Constructor
Farm::Farm(Sprite* sprite) {
    setLayer(1);
    setSize(64, 64);
    allowTerrain(2);
    renderPosition = createChildPosition(0, 0, 64, 64);
    body = sprite;
}

// Get minimap color
Uint32 Farm::getMinimapColor(SDL_PixelFormat* format) {
    return SDL_MapRGBA(format, 255, 0, 0, 255);
}

// Render function
void Farm::render(State* state) {
    Camera* camera = state->camera;
    if (camera->isVisible(getRenderPosition())) {
        drawPosition(state);
        // renderPosition->draw(state);
        body->image->render(body->getRect(), camera->translate(getRenderPosition()));
    }
}
