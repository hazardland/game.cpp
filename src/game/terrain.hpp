#ifndef GAME_TERRAIN
#define GAME_TERRAIN

using namespace std;

#include <vector>
// #include <map>
// #include <random>

class Terrain {
    public:
        int layer;
        vector<int> color;
    Terrain (int layer, vector<int> color) {
        this->layer = layer;
        this->color = color;
    }
    // int getPlain() {
    //     return plain[random(0, plain.size()-1)];
    // }
    // int getEdge(string key) {
    //     if (!edges.contains(key)) {
    //         cout << "missing " << key << "\n";
    //         return 0;
    //     }
    //     return edges[key][random(0, edges[key].size()-1)];
    // }

};

#endif
