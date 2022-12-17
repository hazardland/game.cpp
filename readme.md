# Setup things
This is for me because I every time forget how to setup dev env on windows
![](/doc/images/main.png)

## Windows
1. Follow msys64 installation instructions here https://code.visualstudio.com/docs/languages/cpp#_example-install-mingwx64

    I ussually install it in ```d:\src\msys64``` folder

    a. So I download msys64 installer and run it
    
    b. The first command to run is ```pacman -S mingw-w64-ucrt-x86_64-gcc```
    
    c. Second command for the moment is ```pacman -S --needed base-devel mingw-w64-x86_64-toolchain``` default=all so to install everything because idk what exactly is needed
    
    d. Then adding to path is pain in the ass 
        
    ```D:\app\msys64\ucrt64\bin``` - For some unknow reasons
    
    ```D:\app\msys64\mingw64\bin``` - From tutorial
    
    ```D:\app\msys64\usr\bin``` - From docs
Usually this three paths solve everything

2. Second task is to setup cpp shared lib and include folder which I usuall setup in ```d:\src\cpp``` where two folders reside ```lib``` and ```include```
```
├───include
│   └───SDL2
└───lib
    ├───cmake
    │   └───SDL2
    └───pkgconfig
```
So for now I am a bit lazy to instruct how to download SDL libs and includes and organize in folder let us do ubuntu instead of it then

## Ubuntu
1. Of course ```sudo apt install g++```

2. ```sudo apt install libsdl2-dev```

3. ```sudo apt install libsdl2-image-dev libsdl2-ttf-dev```

    ```g++ -std=c++17 -O3 -m64 -Isrc -s -o main main.cpp src/game/*.cpp  -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf```


# Hello world - Window, Scene and Image classes

The example is very impractical and things are done rather more easaly, but first let us see how things work under the hood.

Our goal with hellow world is to just display static image in the center of window:

![](/doc/images/hello_world.png)

The source sprite we are using looks like that:

![](/doc/images/planet.png)

(Which I generated from the planet sprite generator https://deep-fold.itch.io/pixel-planet-generator idk whoever made it but cool job)

It contains planet animation in 6x4 (cells x rows) spritesheet where the size of each frame is 100x100 px

In hello world example we will use only first frame to render:

```c++
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
```

In this example we learned that there are Window, Scene and Image classes

# Sprite class
Let us advance a bit and see what Sprite class does under the hood before we see how to do things easyer

The example will produce this:

![](/doc/images/hello_world_2.png)

Condsider that the example only depicts how sprite class operates but *this is not the way animation should be done*:

```c++
#include <game/window.hpp>
#include <game/scene.hpp>
#include <game/sprite.hpp>

class MyScene : public Scene {
    using Scene::Scene;

    public:

    // Sprite helds image and some data about frames
    Sprite* sprite;
    SDL_Rect position;

    int currentFrame = 0;

    virtual void prepare() {
        // Create sprite instanse with default config
        sprite = new Sprite(
            new Image(renderer, "doc/images/planet.png"),
            100,
            100
        );

        //
        sprite->addClip(
            1, // Clip index
            1, // Start row in sprite sheet
            1, // Start cell in sprite sheet
            24  // Frame count to generate from row, cell
                // We know our sprite contains 6x4 frames so 24 is total frame count
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

        // Increase current frame
        currentFrame++;
        // Reset current frame to 0 if it becomes 24
        if (currentFrame == sprite->clips[1]->getFrameCount()) {
            currentFrame = 0;
        }
    }

    virtual void render(State* state) {
        clear();

        // Here we specify what to render from image with &frame
        // and where to render on scene with &position
        sprite->image->render(
            // From clip with index 1
            sprite->clips[1]->getFrame(currentFrame)->getRect(),
            &position
        );

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
```

The GIF does not depict how fast the planet spins because we just update sprites ```currentFrame``` number per each frame which are like 300 per second

# Animation

Now let us use animation class to gently handle animations with respet of delta time (elapsed milliseconds from last frame)

![](/doc/images/animation.jpg)


The code is still not the final of how things should be organized but one last class to go:
```c++
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
```

# Dev Blog

From this point we depict the process of implementation and research related to various aspects of framework

# Map generation
## Perlin Noise
Just generate map with x,y for cycle and random appeared to be too complex task, first of all big title games use procedural map generation using random noise which in its turn is a complex algorithm which was invented by some guy named Perlin for movie Tron and the algorithm was called Perlin Noise but eventually it was slow so he came up with new version of algorithm called Simplex Noise but patented it for 3 dimensional use, after which some dudes made OpenSimplexNoise

So first I searched C++ implementations of it, basically none of them work or did not work with C++ 17 standard or had to many dependencies, then after trials and errors one repo with 21 stars appeared to be promising but lacking examples

This is the initial noise:

![](/doc/images/noise1.gif)

Generated by:
```c++
srand(clock);
float intensity = 0.01;
OpenSimplexNoise::Noise noise{rand()}; // Init library with seed
int alpha;
for (int y=0; y<width; y++) {
    for (int x=0; x<height; x++)
    {
        // Use library to generate value for x,y 
        // And scale it from -1 .. 1 to 1 .. 255
        alpha = (noise.eval(x*intensity, y*intensity) + 1) / 2.0  * 255.0;
        terrain->setPixel(x, y, 255, 255, 255, alpha);       
    }
}
```

When the right path was visible I implemented minimap, so plan is that first we generate map plan aka minimap and based on minimap regular map is generated using sprites.

This is how terrain plan is generated in ```example_scene_map.hpp```:
```c++
class TestScenePerlin : public Scene {
    using Scene::Scene;
    Terrain* terrain;
    int ticks;
    public:
    virtual void prepare() {
        terrain = new Terrain(renderer, 512, 512, 1);
        objects.push_back(terrain);
    }
    void update(State* state) {
        if (SDL_GetTicks()-ticks>1000) {
            srand(clock());
            terrain->generate(rand(), 0.01, 6, TERRAIN_RANGES, TERRAIN_COLORS);
            ticks = SDL_GetTicks();
        }
    }
};
```
![](/doc/images/noise2.gif)

Where the different terrain colors and noise ranges are defined like this:

```c++
// We will have 6 terrain types
int TERRAIN_COLORS[6][3] = {
    {51, 51, 255},
    {0, 0, 255},
    {0, 153, 0},
    {255, 153, 51},
    {96, 96, 96},
    {255, 255, 255}
};

// And we declare noise range per terrain type
float TERRAIN_RANGES[6] = {
    0.3,
    0.35,
    0.5,
    0.7,
    0.9,
    1
};
```

## Tiled map smooth edges
Little did I know that biggest trouble was steal ahead: I grabbed Warcraft 2 map tile sprite and just converted terrain number into tail sprite frame number, added some terrain minimap to scroll over the map and this is what showed up:

![](/doc/images/map1.gif)

But this edges, they need to transition smoothly and there is no answer for that in the Google, so I started researching tile variations by myself. It appears these are the possible tile transitions if you transition it with only corner transition rule which just looks like an IQ test question:

![](/doc/images/transitions.png)

I need only blue ones because I eleminated 1 square layout

So I came up with complicated but nice algorithm I was lazy to check all cases and I risked my time just to implement it and the result was unbelivable also the algorithm run without error right in the first compilation:

![](/doc/images/map2.png)

So the map has 3 type of tiles water, ice and snowy ground and the algorithm calculates transitions between them and picks right tiles for it

The algirithm can be found in ```src/game/tile.hpp``` class

It took like couple of days but it was totally worth it, before commiting myself into it I made sure that it was a problem worth solving and I checked if other devs had trouble implementing it, it appears it is significant problem in 2d game developement and Factorio developers had various attempts to solve it https://www.factorio.com/blog/post/fff-344 here you can read more about their adventure