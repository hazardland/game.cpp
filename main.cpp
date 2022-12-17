#include <game/window.hpp>
#include <game/scene.hpp>
#include <game/animation.hpp>

// This is not the final form yet of how things should be organized
class MyScene : public Scene {
    using Scene::Scene;

    public:

    // Animation wraps sprite and clips
    // Can play various clips from the sprite
    // Like imagine Sprite has MOVE and ATTACK clips
    Animation* animation;
    SDL_Rect position;

    int currentFrame = 0;

    virtual void prepare() {

        // Sprites is the map with integers where you can store your sprite collection
        // Because you just need to load sprite once and then reuse in different objects
        sprites[1] = (new Sprite(
            new Image(renderer, "doc/images/planet.png"),
            100,
            100,
            // This is new: Default pause per frame 60 miliseconds for this spritesheet
            // Higher value causes slow animation
            60
        ))->addClip(
            1, // Clip index
            1, // Start row in sprite sheet
            1, // Start cell in sprite sheet
            24  // Frame count to generate from row, cell
                // We know our sprite contains 6x4 frames so 24 is total frame count
        );
        
        // Create animation instanse with sprite instance
        animation = new Animation(
            sprites[1],
            1
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

        // At this point animation is playing default clip
        // And here we just update with delta time elapsed from last frame
        // With delta animaitons will play always with same speed no 
        // matter how many frames per second we do have 
        animation->update(state->clock->delta);

    }

    virtual void render(State* state) {
        clear();

        // Here we specify what to render from image with &frame
        // and where to render on scene with &position
        animation->render(&position);

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