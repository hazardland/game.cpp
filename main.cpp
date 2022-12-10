using namespace std;
#include<stdio.h>

#include <game/window.hpp>

#include <test_scene_perlin.hpp>

int main(int argc, char** argv){

    Window* window = new Window("", 512, 512);
    window->setScene(new TestScenePerlin(window->window));
    return window->run();

}
