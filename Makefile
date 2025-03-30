# Detect Windows environment
ifeq ($(OS),Windows_NT)
    WINDOWS := 1
else
    WINDOWS := 0
endif

PKGS := sdl3 sdl3-image

# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++20 -O3 -m64 -Isrc -s -D__STDC_WANT_LIB_EXT1__=1 `pkg-config --cflags $(PKGS)`
LDFLAGS := `pkg-config --libs $(PKGS)` -lwebsockets -lssl -lcrypto -lz -lpthread

# Add -lmingw32 first for Windows
ifeq ($(WINDOWS), 1)
# CXXFLAGS += -Id:/src/cpp/include -Ld:/src/cpp/lib -s
    LDFLAGS := -lmingw32 -lws2_32 $(LDFLAGS)
endif

# Build directories
BUILD_DIR := build
SRC := main.cpp $(wildcard src/game/*.cpp) $(wildcard src/examples/*.cpp)
OBJ := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRC))

# Create directories for object files
OBJ_DIRS := $(sort $(dir $(OBJ)))

# Default target
all: $(BUILD_DIR) $(OBJ_DIRS) main

# Linking step
main: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ) $(LDFLAGS)

# Compilation step (separate object files in build/)
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR) $(OBJ_DIRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensure build/ directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Ensure all required object directories exist
$(OBJ_DIRS):
	mkdir -p $@

# Clean command
clean:
	rm -rf main $(BUILD_DIR)
