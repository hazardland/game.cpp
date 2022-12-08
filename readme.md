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

    ```g++ -std=c++17 -O3 -m64 -Isrc  -s -o main main.cpp -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf```


# How it works
Here is how animation works
![](/doc/images/animation.jpg)

We first start with preparing of Sprite in the scene ```prepare``` method where we have ```renderer``` instance present 
```c++
Sprite* sprite = new Sprite(
    new Image(renderer, "footman.png"),
    72, // Default frame width
    72, // Default frame height
    100, // Default frame pause
    true // Default sprite read orientation (true == vertical read)
);
```

Then let us define some constants somewhere:
```c++
// Define clip keys
const UNIT_MOVE_UP = 1;
const UNIT_MOVE_DOWN = 1;

sprite->addClip (
    UNIT_MOVE_UP, // Clip name index 
    1, // Start frame cell
    2, // Start frame row
    4, // Number of frames
    true, // Flip x of frame
    false // Flip y of frame
);

// Add some more clips
sprite->addClip (UNIT_MOVE_DOWN, 5, 2, 4);
sprite->addClip (UNIT_MOVE_RIGHT, 3, 2, 4);
sprite->addClip (UNIT_MOVE_LEFT, 3, 2, 4, true);

```

Now that we have sprite with four clips we can create dumb animation

```c++
animation = new Animation(
    sprite, // Our sprite
    UNIT_STAND_DOWN // Default clip from sprite
);
```

During scene update we can call

```c++
// The update will manage to play the clip frame by frame
// By configured delayes with respect to delta
animation->update(delta); // Where delta is provided in scene->update(delta)
```

And we render it somwhere in scene->render(...)
```c++
// Here goes some logic which manages the frame position on screen
SDL_rect position;

// And render the current active frame on screen
animation->render(&position);
```
