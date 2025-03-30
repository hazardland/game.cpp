// File: scene.h

#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <map>
#include <vector>

#include <SDL3/SDL.h>

#include "game/client.h"

class Object;
class Minimap;
class Map;
class Sprite;
class State;
class Window;

/**
 * @brief The Scene class is responsible for managing and rendering objects, sprites,
 * and other visual elements within the game's scenes. It contains a list of visible objects,
 * details about the scene's dimensions, the window and renderer context, and a map and
 * minimap for navigational purposes.
 */
class Scene {
    // A list of objects that are currently visible in the scene
    std::vector<Object*> visibleObjects;
    // Vector of all objects in the scene, vectors are fast during iteration
    std::vector<Object*> objects;
    // For object lookup by id
    std::unordered_map<uint32_t, Object*> objectById;
public:
    Window* window;

    // The width and height of the scene
    int width;
    int height;

    // SDL Renderer and Window context
    SDL_Renderer *renderer;
    // SDL_Window* window;

    // Map of sprite ID to Sprite object
    std::map<int, Sprite*> sprites;


    // Pointer to a Map object, which represents the game's world map
    Map* map;

    // Pointer to a Minimap object, which provides a simplified overview of the game's world map
    Minimap* minimap = nullptr;

    /**
     * @brief Constructor that initializes the Scene object
     *
     * @param window An SDL_Window pointer to the window where the Scene will be displayed
     * @param width An integer representing the width of the Scene
     * @param height An integer representing the height of the Scene
     */
    Scene(Window* window, int width=0, int height=0);

    /**
     * @brief Sets the size of the Scene
     *
     * @param width An integer representing the width of the Scene
     * @param height An integer representing the height of the Scene
     */
    void setSize(int width, int height);

    /**
     * @brief Virtual function to prepare the Scene
     */
    virtual void prepare(State* state);

    /**
     * @brief Updates the state of the Scene
     *
     * @param state A State object that represents the current state of the game
     */
    virtual void update(State* state);

    /**
     * @brief Renders the Scene
     *
     * @param state A State object that represents the current state of the game
     */
    virtual void render(State* state);

    /**
     * @brief Clears the Scene
     */
    virtual void clear();

    /**
     * @brief Builds the Scene
     *
     * @param state A State object that represents the current state of the game
     */
    virtual void build(State* state);

    /**
     * @brief Presents the Scene
     *
     * @param delay An integer representing the delay before the Scene is presented
     */
    virtual void present(int delay=1);

    /**
     * @brief Adds an object to the Scene
     *
     * @param obj An Object pointer representing the object to be added to the Scene
     */
    void addObject(Object* obj);

    
    /**
     * @brief Adds an object to the Scene
     *
     * @param obj An Object pointer representing the object to be added to the Scene
     */
    void addObject(Object* obj, uint32_t id);

    /**
     * @brief Get object by id
     *
     * @param id Id of the object
     */
    Object* getObject(int id);

    /**
     * @brief Removes object from all places, Note if it is also Unit you should remove it from map by yourself
     *
     * @param obj An Object pointer representing the object to be added to the Scene
     */
    void removeObject(Object* obj);
    
    /**
     * @brief Removes object from all places, Note if it is also Unit you should remove it from map by yourself
     *
     * @param id An Object pointer representing the object to be added to the Scene
     */
    void removeObject(int id);
        
    
    /**
     * @brief Destructor that cleans up the Scene object
     */
    virtual ~Scene();

protected:
    Client* client = nullptr;
};

#endif // GAME_SCENE_H
