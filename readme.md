# Setup things
This is for me because I every time forget how to setup dev env on windows
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

    g++ -std=c++17 -O3 -m64 -Isrc  -s -o main main.cpp -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

