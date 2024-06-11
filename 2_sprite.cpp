#include <game/window.h>
#include <game/scene.h>
#include <game/sprite.h>
#include <game/clip.h>
#include <game/image.h>

class MyScene : public Scene {
    using Scene::Scene;

    public:

    // Sprite helds image and some data about frames
    Sprite* sprite;
    SDL_Rect position;

    int currentFrame = 0;

    virtual void prepare() {
        // Create sprite instanse with default config
        sprite = new Sprite(
            new Image(renderer, "doc/images/planet.png"),
            100,
            100
        );

        //
        sprite->addClip(
            1, // Clip index
            1, // Start row in sprite sheet
            1, // Start cell in sprite sheet
            24  // Frame count to generate from row, cell
                // We know our sprite contains 6x4 frames so 24 is total frame count
        );

        // Scale the frame a bit from its original size
        position.w = 200;
        position.h = 200;
    }

    virtual void update(State* state) {
        // Here we center image based on scene width and height
        // No matter window size it will always stay in center
        position.x = this->width/2 - position.w/2; 
        position.y = this->height/2 - position.h/2;

        // Increase current frame
        currentFrame++;
        // Reset current frame to 0 if it becomes 24
        if (currentFrame == sprite->clips[1]->getFrameCount()) {
            currentFrame = 0;
        }
    }

    virtual void render(State* state) {
        clear();

        // Here we specify what to render from image with &frame
        // and where to render on scene with &position
        sprite->image->render(
            // From clip with index 1
            sprite->getClip(1)->getFrame(currentFrame)->getRect(),
            &position
        );

        present();
    }

};

int main(int argc, char** argv){
    // Just create the window
    Window* window = new Window("Hello World", 800, 600);
    // Pass our scene instanse
    window->setScene(new MyScene(window->window));
    // Do the thing
    return window->run();
}