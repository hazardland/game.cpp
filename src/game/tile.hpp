#ifndef GAME_TILE
#define GAME_TILE

using namespace std;

#include <vector>
#include <map>

#include <game/terrain.hpp>
#include <game/map.hpp>

class Tile {
    public:
        vector<int> plain;
        map<string, vector<int>> edges;
    Tile (vector<int> plain, map<string, vector<int>> edges={}) {
        this->plain = plain;
        this->edges = edges;
    }
    int random(int min, int max) {
        return rand() % (max-min+1) + min;   
    }
    int getPlain() {
        return plain[random(0, plain.size()-1)];
    }
    int getEdge(string key) {
        if (key=="1001") cout << key << "\n";
        return edges[key][random(0, edges[key].size()-1)];
    }
    int getTile(Terrain* terrain, int x, int y) {
        // Get corner sum
        int type = terrain->grid[x][y];
        //   x x x
        // y 0 0 0
        // y 0 0 0
        // y 0 0 0
        int borders[] = {
            1, 1, 1, // 0 1 2 
            1, 1, 1, // 3 4 5
            1, 1, 1  // 6 7 8
        };
        int corners[] = {
            0, 0, // 0 1
            0, 0  // 2 3
        };
        if (x!=0 && y!=0){
            borders[0] = terrain->grid[x-1][y-1]<type ? 0 : 1;
        }
        if (y!=0){
            borders[1] = terrain->grid[x  ][y-1]<type ? 0 : 1;
        }
        if (y!=0 && x<terrain->width-1) {
            borders[2] = terrain->grid[x+1][y-1]<type ? 0 : 1;
        }
        if (x!=0) {
            borders[3] = terrain->grid[x-1][y  ]<type ? 0 : 1;
        }
        if (x<terrain->width-1) {
            borders[5] = terrain->grid[x+1][y  ]<type ? 0 : 1;
        }
        if (x!=0 && y<terrain->height-1) {
            borders[6] = terrain->grid[x-1][y+1]<type ? 0 : 1;
        }
        if (y<terrain->height-1){
            borders[7] = terrain->grid[x  ][y+1]<type ? 0 : 1;
        }
        if (x<terrain->width-1 && y<terrain->height-1) {
            borders[8] = terrain->grid[x+1][y+1]<type ? 0 : 1;
        }
        corners[0] = borders[3] + borders[0] + borders[1];  
        corners[1] = borders[1] + borders[2] + borders[5];  
        corners[2] = borders[3] + borders[6] + borders[7];  
        corners[3] = borders[7] + borders[8] + borders[5];

        int corner = 0;
        int min = corners[0];

        for(int i=1; i<4; i++)
        {
            if(corners[i]<min)
            {
                min = corners[i];
                corner = i;
            }
        }

        // int borders[] = {
        //     1, 1, 1, // 0 1 2 
        //     1, 1, 1, // 3 4 5
        //     1, 1, 1  // 6 7 8
        // };
        // int corners[] = {
        //     0, 0, // 0 1
        //     0, 0  // 2 3
        // };
        
        string result;
        if (corner==0) {
            result = 
            to_string(borders[0]) + 
            to_string(borders[1]) + 
            to_string(borders[3]) + 
            to_string(borders[4]);
        }
        if (corner==1) {
            result = 
            to_string(borders[1]) + 
            to_string(borders[2]) + 
            to_string(borders[4]) + 
            to_string(borders[5]);

        }
        if (corner==2) { 
            result = 
            to_string(borders[3]) + 
            to_string(borders[4]) + 
            to_string(borders[6]) + 
            to_string(borders[7]);

        }
        if (corner==3) {
            result = 
            to_string(borders[4]) + 
            to_string(borders[5]) + 
            to_string(borders[7]) + 
            to_string(borders[8]);            
        }

        // printf("-- x: %d y: %d c: %d\n%d %d %d\n%d %d %d\n%d %d %d\n",
        // x, y, corner, borders[0],borders[1], borders[2], borders[3], borders[4], borders[5], borders[6], borders[7], borders[8]        
        // );
        
        // Mark bad tile
        if (result=="1001") {
            terrain->setPixel(x, y, 0, 0, 0, 255);
        }

        if (result.compare("1111")==0) {
            return getPlain();
        }
        return getEdge(result);
    }
};

#endif
