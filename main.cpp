using namespace std;

#include <game/input.h>
#include <game/camera.h>
#include <game/window.h>

#include <testscene.h>

int main(int argc, char** argv){

    Window* window = new Window("Game Title", 500, 500);
    // Window can take custom scene
    window->setScene(new TestScene(window->window));
    // Window can take custom input
    window->setInput(new Input());
    window->setCamera(new Camera());
    return window->run();

}
