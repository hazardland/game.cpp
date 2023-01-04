using namespace std;
#include<stdio.h>

#include <game/window.hpp>

#include <examples/war2.hpp>


int main(int argc, char** argv){
    Window* window = new Window("", 1280, 720);
    window->setScene(new MapScene(window->window));
    return window->run();
}
