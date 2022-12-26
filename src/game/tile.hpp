#ifndef GAME_TILE
#define GAME_TILE

using namespace std;

#include <vector>

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
        if (!edges.contains(key)) {
            cout << "missing " << key << "\n";
            return 0;
        }
        return edges[key][random(0, edges[key].size()-1)];
    }

};

#endif
