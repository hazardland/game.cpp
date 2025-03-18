#include <game/window.h>
#include <game/scene.h>
#include <game/animation.h>
#include <game/object.h>
#include <game/state.h>
#include <game/camera.h>
#include <game/clock.h>
#include <game/image.h>
#include <game/sprite.h>

int PLANET_SPRITE = 1;
int PLANET_SPRITE_SPIN_CLIP = 1;

class Planet: public Object {
    public:
    Animation* animation;
    Planet(Sprite* sprite) {
        animation = new Animation(
            sprite,
            1
        );
        setSize(200, 200);
    }

    virtual void update(State* state) {
        setPosition (state->camera->getWidth()/2 - getWidth()/2,
                     state->camera->getHeight()/2 - getHeight()/2);
        animation->update(state->clock->delta);
    }

    virtual void render(State* state) {
        animation->render(getPosition());
    }

};

class MyScene : public Scene {
    using Scene::Scene;

    public:

    virtual void prepare() {

        // Define sprite
        sprites[PLANET_SPRITE] = (new Sprite(
            new Image(renderer, "doc/images/planet.png"),
            100, // Frame width
            100 // Frame height
        ))->addClip(
            PLANET_SPRITE_SPIN_CLIP, // Clip index
            1, // Start frame row in sprite sheet
            1, // Start frame cell in sprite sheet
            24  // Frame count to generate from row, cell starting position
        );
        
        // Create Planet object
        addObject(new Planet(sprites[PLANET_SPRITE]));
    }

};

int main(int argc, char** argv){
    Window* window = new Window("Hello World", 800, 600);
    window->setScene(new MyScene(window));
    return window->run();
}