#include "examples/farm.h"

// Constructor
Farm::Farm(Sprite* sprite) {
    // printf("Farm %d,%d -> %d,%d\n", gridFromX, gridFromY, gridToX, gridToY);
    setLayer(1);
    setSize(64, 64);
    // printf("Farm %d,%d -> %d,%d\n", gridFromX, gridFromY, gridToX, gridToY);

    allowTerrain(2);
    renderPosition = createChildPosition(0, 0, 64, 64);
    body = sprite;
    setMinimapColor ({255, 0, 0, 255});
}

// Render function
void Farm::render(State* state) {
    Camera* camera = state->camera;
    if (camera->isVisible(getRenderPosition())) {
        // drawPosition(state);
        // renderPosition->draw(state);
        body->image->render(body->getRect(), camera->translate(getRenderPosition()));
    }
}
