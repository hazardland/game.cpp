using namespace std;

#include <game/window.h>
#include <game/input.h>

#include <war2.h>

int main(int argc, char** argv){

    Window* window = new Window("Game Title", 500, 500);
    window->setScene(new Game(window->window));
    window->setInput(new Input());
    return window->run();

}
