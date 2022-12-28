#ifndef GAME_TERRAIN
#define GAME_TERRAIN

using namespace std;

#include <vector>

class Terrain {
    public:
        vector<int> plain;
        map<string, vector<int>> edges;
        int layer;
        vector<int> color;
    Terrain (int layer, vector<int> color, vector<int> plain, map<string, vector<int>> edges={}) {
        this->layer = layer;
        this->color = color;
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
