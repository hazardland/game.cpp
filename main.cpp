using namespace std;

#include <algorithm>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>


// Represents a point on the grid
struct Point {
    int x, y;
    Point(int x, int y): x(x), y(y) {}
    bool operator == (const Point & p) const {
        return x == p.x && y == p.y;
    }
};

// Hash function for Points, needed for using them in unordered_map
struct PointHash {
    size_t operator()(const Point & p) const {
        return hash < int > ()(p.x) ^ hash < int > ()(p.y);
    }
};

// Represents a node in the search tree
struct Node {
    Point point;
    int g, h;
    Node(Point point, int g, int h): point(point), g(g), h(h) {}
    int f() const {
        return g + h;
    }
};

// Comparison function for nodes, used in the priority queue
struct NodeCompare {
    bool operator()(const Node & a,
        const Node & b) const {
        return a.f() > b.f();
    }
};

// Returns the Manhattan distance between two points
int manhattanDistance(const Point & a,
    const Point & b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

// Returns the neighbors of a point
vector < Point > getNeighbors(const Point & p,
    const vector < vector < int >> & grid) {
    vector < Point > neighbors;
    const int dx[] = {-1,0,1,0,-1,-1,1,1};
    const int dy[] = {0,1,0,-1,-1,1,-1,1};
    for (int i = 0; i < 8; i++) {
        int x = p.x + dx[i];
        int y = p.y + dy[i];
        if (x >= 0 && x < grid.size() && y >= 0 && y < grid[0].size() && grid[x][y] == 0) {
            neighbors.emplace_back(x, y);
        }
    }
    return neighbors;
}

vector < Point > aStar(const vector < vector < int >> & grid,
    const Point & start,
        const Point & goal) {
    // Set of visited nodes
    unordered_map < Point, Node, PointHash > cameFrom;
    // Set of nodes still to be evaluated
    priority_queue < Node, vector < Node > , NodeCompare > openSet;
    // Set of nodes already evaluated
    unordered_set < Point, PointHash > closedSet;

    openSet.emplace(start, 0, manhattanDistance(start, goal));

    while (!openSet.empty()) {
        Node current = openSet.top();
        openSet.pop();

        // Check if we have reached the goal
        if (current.point == goal) {
            // Construct the path by tracing back the cameFrom map
            vector < Point > path;
            while (current.point != start) {
                path.emplace_back(current.point);
                current = cameFrom[current.point];
            }
            path.emplace_back(start);
            // The path is constructed in reverse order, so we need to reverse it
            reverse(path.begin(), path.end());
            return path;
        }

        closedSet.emplace(current.point);

        for (const Point & neighbor: getNeighbors(current.point, grid)) {
            // Skip already visited points
            if (closedSet.count(neighbor) > 0) continue;

            int tentativeG = current.g + 1; // The cost of moving from the current point to a neighbor is always 1

            // Check if we have already seen this neighbor
            if (cameFrom.count(neighbor) == 0) {
                // This is a new point, so add it to the open set
                openSet.emplace(neighbor, tentativeG, manhattanDistance(neighbor, goal));
                cameFrom[neighbor] = Node(neighbor, tentativeG, manhattanDistance(neighbor, goal));
            } else if (tentativeG < cameFrom[neighbor].g) {
                // We have seen this neighbor before and the current path to it is better, so update the cameFrom map
                cameFrom[neighbor] = Node(neighbor, tentativeG, manhattanDistance(neighbor, goal));
            }
        }
    }

    // If we reach here, it means that we couldn't find a path to the goal
    return {};
}

int main() {
    // Example usage
    vector<vector<int>> grid = {{0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 1, 1, 0}, {0, 0, 0, 0, 0}};
    Point start(0, 0);
    Point goal(4, 4);
    vector < Point > path = aStar(grid, start, goal);
    for (const Point & p: path) {
        cout << "(" << p.x << ", " << p.y << ") ";
    }
    cout << endl;
    return 0;
}