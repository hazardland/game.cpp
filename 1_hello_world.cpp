#include <game/window.hpp>
#include <game/scene.hpp>
#include <game/image.hpp>

// We will need scene to exend to setup our custom things
class MyScene : public Scene {
    using Scene::Scene;

    public:

    // We will store our loaded image here
    Image* image;
    // Here we store what we need to crop from image
    SDL_Rect frame;
    // Here we store where and what size we need to display
    SDL_Rect position;

    virtual void prepare() {
        // Load the image
        image = new Image(renderer, "doc/images/planet.png");
        
        // The image is sprite sheet
        // But we only need first frame from it
        frame.x = 0;
        frame.y = 0;
        frame.w = 100;
        frame.h = 100;

        // Scale the frame a bit from its original size
        position.w = 200;
        position.h = 200;
    }

    virtual void update(State* state) {
        // Here we center image based on scene width and height
        // No matter window size it will always stay in center
        position.x = this->width/2 - position.w/2; 
        position.y = this->height/2 - position.h/2; 
    }

    virtual void render(State* state) {
        clear();

        // Here we specify what to render from image with &frame
        // and where to render on scene with &position
        image->render(&frame, &position);

        display();
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