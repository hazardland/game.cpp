# Detect Windows environment
ifeq ($(OS),Windows_NT)
    WINDOWS := 1
else
    WINDOWS := 0
endif

all:
ifeq ($(WINDOWS), 1)
	g++ -std=c++20 -O3 -m64 -Isrc -Id:/src/cpp/include -Ld:/src/cpp/lib -s -o main main.cpp src/game/*.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
else
	g++ -std=c++20 -O3 -m64 -Isrc -Id:/src/cpp/include -Ld:/src/cpp/lib -s -o main main.cpp src/game/*.cpp -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
endif
