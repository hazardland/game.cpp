all:
	g++ -std=c++20 -O3 -m64 -Isrc -Id:/src/cpp/include -Ld:/src/cpp/lib -s -o 4_object 4_object.cpp src/game/*.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf