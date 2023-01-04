#include <game/window.hpp>
#include <game/scene.hpp>
#include <game/animation.hpp>

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
        setPosition (state->camera->width/2 - getWidth()/2,
                     state->camera->height/2 - getHeight()/2);
        animation->update(state->clock->delta);
    }

    virtual void render(State* state) {
        animation->render(&position);
    }

};

class MyScene : public Scene {
    using Scene::Scene;

    public:

    virtual void prepare() {

        // Define sprite
        sprites[1] = (new Sprite(
            new Image(renderer, "doc/images/planet.png"),
            100, // Frame width
            100, // Frame height
            60 // Frame pause
        ))->addClip(
            1, // Clip index
            1, // Start row in sprite sheet
            1, // Start cell in sprite sheet
            24  // Frame count to generate from row, cell
        );
        
        // Create Planet object
        objects.push_back(new Planet(sprites[1]));
    }

};

int main(int argc, char** argv){
    Window* window = new Window("Hello World", 800, 600);
    window->setScene(new MyScene(window->window));
    return window->run();
}