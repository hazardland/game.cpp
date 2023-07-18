#ifndef GAME_UNIT
#define GAME_UNIT

using namespace std;

#include <iostream>
#include <vector>
#include <algorithm>


#include <SDL2/SDL_image.h>

#include <game/position.hpp>
#include <game/object.hpp>
#include <game/state.hpp>
#include <game/camera.hpp>
#include <game/map.hpp>
#include <game/minimap.hpp>
#include <game/scene.hpp>

/*
    All methods here work only for on screen position
    Have your own float x, float y to track object's game world position 
*/
class Unit: public Object {

    int lastCellLeft, lastCellTop, lastCellRight, lastCellBottom = -1;

    public:

        Map* map;
        Minimap* minimap;
        Scene* scene;

        Position* renderPosition;
        Position* selectPosition;

        std::vector<std::pair<int, int>> cells;


        virtual Unit* setMap(Map* map) {
            this->map = map;
            return this;
        }
        virtual Unit* setMinimap(Minimap* minimap) {
            this->minimap = minimap;
            minimap->addObject(this);
            return this;
        }
        virtual Unit* setScene(Map* map) {
            this->scene = scene;
            return this;
        }

        virtual void render(State* state) {

        }
        virtual SDL_FRect* getRenderPosition() {
            return renderPosition->getSDL_FRect();
        }

        virtual float getRenderX() {
            return renderPosition->getX();
        }

        virtual float getRenderY() {
            return renderPosition->getY();
        }

        virtual float getRenderHeight() {
            return renderPosition->getWidth();
        }

        virtual float getRenderWidth() {
            return renderPosition->getHeight();
        }

        virtual void renderHitbox(State* state) {
            
        }

        virtual SDL_Rect getMinimapRect() {
            return {
                int(getX()/minimap->widthRatio()), 
                int(getY())/minimap->heightRatio(),
                int(getWidth()/minimap->widthRatio()),
                int(getHeight()/minimap->heightRatio())
            };
        }

        void updateMapCells() {
            if (!map) {
                return;
            }

            // Calculate the new cells this object should occupy
            int left = static_cast<int>(getX() / map->tileWidth);
            int top = static_cast<int>(getY() / map->tileHeight);
            int right = static_cast<int>((getX() + getWidth()) / map->tileWidth);
            int bottom = static_cast<int>((getY() + getHeight()) / map->tileHeight);

            // Correct the boundaries if they are out of the map's bounds
            left = max(0, left);
            top = max(0, top);
            right = min(map->tilesPerWidth - 1, right);
            bottom = min(map->tilesPerHeight - 1, bottom);

            if(left == lastCellLeft && top == lastCellTop && right == lastCellRight && bottom == lastCellBottom){
                return;
            }            
            
            // Remove object from its current cells
            for (const auto& cell : cells) {
                map->grid[cell.first][cell.second]->objects[getLayer()].remove(this);
            }
            cells.clear();


            // Add object to the new cells
            for (int i = left; i <= right; i++) {
                for (int j = top; j <= bottom; j++) {
                    map->grid[i][j]->objects[getLayer()].push_back(this);
                    cells.push_back(make_pair(i, j));
                }
            }     

            lastCellLeft = left;
            lastCellTop = top;
            lastCellRight = right;
            lastCellBottom = bottom;        

        }

        bool canMove(float dx, float dy) { //, float dw, float dh
            // Calculate the prospective new position and dimensions
            float newX = getX() + dx;
            float newY = getY() + dy;
            float newWidth = getWidth(); // + dw;
            float newHeight = getHeight(); // + dh;

            // Check if the new position would be outside the map's bounds
            if (newX < 0 || newX + newWidth > map->tilesPerWidth * map->tileWidth ||
                newY < 0 || newY + newHeight > map->tilesPerHeight * map->tileHeight) {
                return false;
            }

            // Calculate the cells this object would occupy after moving
            int left = static_cast<int>(newX / map->tileWidth);
            int top = static_cast<int>(newY / map->tileHeight);
            int right = static_cast<int>((newX + newWidth) / map->tileWidth);
            int bottom = static_cast<int>((newY + newHeight) / map->tileHeight);

            // Correct the boundaries if they are out of the map's bounds
            left = max(0, left);
            top = max(0, top);
            right = min(map->tilesPerWidth - 1, right);
            bottom = min(map->tilesPerHeight - 1, bottom);

            // Check every cell this object would occupy after moving
            for (int i = left; i <= right; i++) {
                for (int j = top; j <= bottom; j++) {

            if (map->grid[i][j]->terrain->layer != getLayer() && getLayer() != 0) {
                return false;
            }
                    // if (map->grid[i][j]->terrain->layer != getLayer() && getLayer() != 0) {
                    //     return false;
                    // }                    
                    // Get the objects in the current cell
                    auto& objects = map->grid[i][j]->objects[getLayer()];

                    // Check every object in the cell
                    for (const auto& object : objects) {
                        // If the object is not the same as this one and they overlap, return false
                        if (object != this && 
                            newX < object->getX() + object->getWidth() &&
                            newX + newWidth > object->getX() &&
                            newY < object->getY() + object->getHeight() &&
                            newY + newHeight > object->getY()) {
                            return false;
                        }
                    }
                }
            }

            // No collisions were found, so return true
            return true;
        }


        void addPosition(float x, float y) override {
            if (canMove(x, y)) {
                Object::addPosition(x, y);
                updateMapCells();
            } else if (canMove(x, 0)) {
                Object::addPosition(x, 0);
                updateMapCells();
            } else if (canMove(0, y)) {
                Object::addPosition(0, y);
                updateMapCells();
            }
        }

        void setPosition(float x, float y) override {
            Object::setPosition(x, y);
            updateMapCells();
        }

        // void setSize(float width, float height) override {
        //     Object::setSize(width, height);
        //     updateMapCells();
        // }

        // void setX(float x) override {
        //     Object::setX(x);
        //     updateMapCells();
        // }

        // void setY(float y) override {
        //     Object::setY(y);
        //     updateMapCells();
        // }

        // void setWidth(float width) override {
        //     Object::setWidth(width);
        //     updateMapCells();
        // }

        // void setHeight(float height) override {
        //     Object::setHeight(height);
        //     updateMapCells();
        // }


        ~Unit() {
            delete renderPosition;
            delete selectPosition;
        }        

};

#endif
