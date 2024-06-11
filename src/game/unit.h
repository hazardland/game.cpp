#ifndef GAME_UNIT
#define GAME_UNIT

#include <vector>
#include <SDL2/SDL_image.h>

#include "game/object.h"

// Forward declaration of classes to avoid circular dependencies.
// class Object;
class Map;
class Minimap;
class Scene;
class State;
class Position;
class Cell;
class Terrain;

/* Class 'Unit' extends the 'Object' class and represents a unit in a game. 
   It includes methods for rendering the unit, updating its map cells, 
   determining its ability to move, and managing its selection state.
*/
class Unit: public Object {
private:
    int lastCellLeft, lastCellTop, lastCellRight, lastCellBottom;
    bool selected;
    std::vector<std::pair<int, int>> cells;

public:
    Map* map;
    Minimap* minimap;
    Scene* scene;
    Position* renderPosition;
    Position* selectPosition;

    // Setter methods
    virtual Unit* setMap(Map* map);
    //virtual Unit* setScene(Map* map);

    // Render methods
    virtual void render(State* state);
    virtual SDL_FRect* getRenderPosition() override;

    //virtual void renderHitbox(State* state);

    // Map cell methods
    void updateMapCells();
    bool canMove(float dx, float dy);

    // Position methods
    void addPosition(float x, float y) override;
    void setPosition(float x, float y) override;

    // Selection methods
    bool isSelected();
    void select();

    // Destructor
    ~Unit();
};

#endif // GAME_UNIT
