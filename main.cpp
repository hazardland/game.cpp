using namespace std;
#include<stdio.h>

#include <game/window.hpp>

#include <example_scene_map.hpp>

int main(int argc, char** argv){
    Window* window = new Window("", 1280, 720);
    window->setScene(new TestScenePerlin(window->window));
    return window->run();
}
