#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>

using namespace std;

struct Node
{
    int x, y;
    int g, h, f;
    Node *parent;

    Node(int x, int y, Node *parent = nullptr) : x(x), y(y), g(100), h(0), f(0), parent(parent) {}

    // min heap
    bool operator<(const Node &other) const
    {
        return f > other.f;
    }
};

int heuristic(Node *a, Node *b)
{
    return abs(a->x - b->x) + abs(a->y - b->y);
}

vector<Node *> get_neighbors(Node *node, vector<vector<int>> &grid)
{
    vector<Node *> neighbors;
    int dx[] = {0, 1, 0, -1};
    int dy[] = {1, 0, -1, 0};

    for (int i = 0; i < 4; ++i)
    {
        int nx = node->x + dx[i];
        int ny = node->y + dy[i];

        if (nx >= 0 && nx < grid.size() && ny >= 0 && ny < grid[0].size())
        {
            neighbors.push_back(new Node(nx, ny, node));
        }
    }

    return neighbors;
}

vector<Node *> reconstruct_path(Node *node)
{
    vector<Node *> path;
    while (node)
    {
        path.push_back(node);
        node = node->parent;
    }
    reverse(path.begin(), path.end());
    return path;
}

vector<Node *> a_star(Node *start, Node *goal, vector<vector<int>> &grid)
{
    priority_queue<Node *> open_set;
    vector<vector<bool>> closed_set(grid.size(), vector<bool>(grid[0].size(), false));

    start->h = heuristic(start, goal);
    start->g = 0;
    start->f = start->h;
    open_set.push(start);

    while (!open_set.empty())
    {
        Node *current = open_set.top();
        open_set.pop();
        cout << current->x << " " << current->y << endl;

        if (current->x == goal->x && current->y == goal->y)
        {
            return reconstruct_path(current);
        }

        closed_set[current->x][current->y] = true;

        for (Node *neighbor : get_neighbors(current, grid))
        {
            if (closed_set[neighbor->x][neighbor->y] == false)
            {cout << "neighbor: " << neighbor->x << " " << neighbor->y << endl;
                int tentative_g = current->g + grid[neighbor->x][neighbor->y] * 100;
                // if corner, add via cost
                // if (current->parent != nullptr && (neighbor->x != current->parent->x && neighbor->y != current->parent->y))
                // {
                //     tentative_g += 1;
                // }
                if(tentative_g >= neighbor->g) continue;
                neighbor->g = tentative_g;
                neighbor->h = heuristic(neighbor, goal);
                neighbor->f = neighbor->h + neighbor->g;
                open_set.push(neighbor);
            }
        }
    }
    return vector<Node *>();
}

int main()
{
    vector<vector<int>> grid = {
        {0, 1, 1, 1, 0},
        {0, 1, 0, 0, 0},
        {0, 0, 0, 1, 0},
        {0, 1, 0, 1, 0},
        {0, 0, 1, 0, 0}};

    Node *start = new Node(4, 3);
    Node *goal = new Node(1, 0);

    vector<Node *> path = a_star(start, goal, grid);

    if (!path.empty())
    {
        cout << "Path found:" << endl;
        for (Node *node : path)
        {
            cout << "(" << node->x << ", " << node->y << ")" << endl;
        }
    }
    else
    {
        cout << "No path found." << endl;
    }

    return 0;
}
