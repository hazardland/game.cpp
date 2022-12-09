using namespace std;

#include <game/input.hpp>
#include <game/camera.hpp>
#include <game/window.hpp>

#include <test_scene.hpp>

int main(int argc, char** argv){

    Window* window = new Window("The Game", 500, 500);
    // Window can take custom scene
    window->setScene(new TestScene(window->window));
    // Window can take custom input
    window->setInput(new Input());
    window->setCamera(new Camera());
    return window->run();

}
