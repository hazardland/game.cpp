using namespace std;
#include<stdio.h>

#include <game/window.h>

#include <examples/war2.h>


int main(int argc, char** argv){
    printf("Starting...\n");
    Window* window = new Window("", 1280, 720);
    window->setScene(new Warcraft(window->window));
    return window->run();
}
