#ifndef GAME_MINIMAP
#define GAME_MINIMAP

using namespace std;

#include <iostream>
#include <map>

#include <SDL2/SDL_image.h>


#include <game/object.hpp>
#include <game/state.hpp>
#include <game/drag.hpp>

class Minimap: public Object {

    public:

    SDL_Renderer* renderer;
    SDL_Surface* terrain;
    SDL_Texture* background = NULL;
    SDL_FRect scope;
    SDL_Rect frame;

    Object* map;
    vector<Object*> objects;

    int worldWidth;
    int worldHeight;
    float scale;

    int scopeWidth = 2;
    SDL_Color scopeColor = {50, 205, 50};

    bool modified = false;

    Drag* drag;    

    /**
     * @brief Mini map for terrain, object markers with camera scope 
     * @param renderer Renderer used for drawing 
     * @param width Minimap view width 
     * @param height Minimap view height
     * @param worldWidth Map tile count in width
     * @param worldHeight Map tile count in height
     * @param scale Tile scale on minimap
     * @param map Parent map
    */
    Minimap(SDL_Renderer* renderer, 
           int width,
           int height,
           int worldWidth,
           int worldHeight, 
           float scale,
           Object* map
           ) {

        this->map = map;

        this->worldWidth = worldWidth;
        this->worldHeight = worldHeight;
        this->scale = scale;

        this->renderer = renderer;

        frame.x = 0;
        frame.y = 0;
        frame.w = width;
        frame.h = height;
        setPosition(0, 0);
        setSize(width, height);
        terrain = SDL_CreateRGBSurfaceWithFormat(0, worldHeight * scale, worldWidth * scale, 0, SDL_PIXELFORMAT_RGBA32);
        prepare();


        drag = new Drag(getPosition(), false, true);

    }

    /**
     * 
    */
    void setTerrain(int x, int y, int red, int blue, int green) {
        SDL_Rect rect;
        rect.x = x*scale;
        rect.y = y*scale;
        rect.h = scale<1?1:scale;
        rect.w = scale<1?1:scale;
        SDL_FillRect(terrain, &rect, SDL_MapRGB(terrain->format, red, blue, green));
        modified = true;
    }

    void addObject(Object* object) {
        objects.push_back(object);
    }

    void prepare() {
        if (background!=NULL) {
            printf("Destroyed minimap background\n");
            SDL_DestroyTexture(background);
        }
        background = SDL_CreateTextureFromSurface(renderer, terrain);
        modified = false;
    }

    virtual void update(State* state) {
        Mouse* mouse = state->event->mouse;
        Camera* camera = state->camera;

        drag->update(state);

        bool manualPick = false;
        if ((mouse->leftClick || mouse->leftDragActive) && mouse->inside(getPosition())) {
            // state->camera->x = ((mouse->x - getX())/scale) * (map->getWidth()/worldWidth) - camera->width/2; 
            // state->camera->y = ((mouse->y - getY())/scale) * (map->getHeight()/worldHeight) - camera->height/2;

            camera->x = ((mouse->x - getX() + frame.x)) * (map->getWidth()/(worldWidth*scale)) - camera->width/2; 
            camera->y = ((mouse->y - getY() + frame.y)) * (map->getHeight()/(worldHeight*scale)) - camera->height/2;
            manualPick = true;
        }


        // Prepare scope rectangle
        scope.x = camera->x / (map->getWidth()/(worldWidth * scale));
        scope.y = camera->y / (map->getHeight()/(worldHeight * scale));
        scope.w = camera->width / (map->getWidth()/(worldWidth * scale));
        scope.h = camera->height / (map->getHeight()/(worldHeight * scale));

        if (!manualPick) {

            frame.x = scope.x + scope.w/2 - frame.w/2;
            frame.y = scope.y + scope.h/2 - frame.h/2;

            if (frame.x+frame.w > worldWidth*scale) {
                frame.x -= ((frame.x+frame.w) - (worldWidth*scale)); 
            }

            if (frame.y+frame.h > worldHeight*scale) {
                frame.y -= ((frame.y+frame.h) - (worldHeight*scale)); 
            }

            if (frame.x<0) {
                frame.x = 0;
            }
            if (frame.y<0) {
                frame.y = 0;
            }

        }
        scope.x += getX() - frame.x;
        scope.y += getY() - frame.y;

    }

    virtual void render(State* state) {
        Camera* camera = state->camera;

        if (modified) {
            prepare();
        }

        // Render terrain
        SDL_RenderCopyF(renderer, background, &frame, getPosition());

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        //Render dinamic objects
        SDL_FRect* rects;
        rects = new SDL_FRect[objects.size()];
        SDL_FRect* rect;
        int i = 0;
        for (Object* object: objects) {
            rect = &rects[i];
            rect->x = object->getX() / (map->getWidth()/(worldWidth * scale));
            rect->y = object->getY() / (map->getHeight()/(worldHeight * scale));
            // rect.h = scale<1?1:scale;
            // rect.w = scale<1?1:scale;
            rect->h = 1;
            rect->w = 1;
            translate(rect);
            if (isVisible(rect)) {
            }
            i++;
        }
        SDL_RenderFillRectsF(renderer, rects, objects.size());
        //delete rect;
        delete rects;
        
        //Render scope rectangle


        if (map!=NULL) {
            renderRectColor(&scope, 255, 255, 255, 50);
            renderRectBorder(&scope, 102, 5, 51, 255);
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    }

    void renderRectColor(SDL_FRect* rect, int red, int green, int blue, int alpha=255) {
        SDL_SetRenderDrawColor(renderer, red, blue, green, alpha);
        SDL_RenderFillRectF(renderer, rect);
    }

    void renderRectBorder(SDL_FRect* rect, int red, int green, int blue, int alpha=255) {
        SDL_SetRenderDrawColor(renderer, red, blue, green, alpha);
        SDL_RenderDrawRectF(renderer, rect);
    }

    bool isVisible(SDL_FRect* position) {
        if (position->x + position->w  > frame.x && 
            position->y + position->h > frame.y &&
            position->x < frame.x + frame.w && 
            position->y < frame.y + frame.h
        ) {
            return true;
        }
        return false;
    }
    
    void translate(SDL_FRect* position) {
        position->x += getX() - frame.x; 
        position->y += getY() - frame.y;
    }

};


#endif
