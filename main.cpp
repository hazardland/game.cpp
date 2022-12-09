using namespace std;

#include <game/state.hpp>
#include <game/event.hpp>
#include <game/camera.hpp>
#include <game/window.hpp>

#include <test_scene.hpp>

int main(int argc, char** argv){

    Window* window = new Window("The Game", 500, 500);
    window->setScene(new TestScene(window->window));
    return window->run();

}
