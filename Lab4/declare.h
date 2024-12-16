#ifndef DECLARE_H
#define DECLARE_H
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <queue>
#include <limits>
#include <algorithm>

using namespace std;

struct POS // position
{
    int x;
    int y;
};

enum GRIDTYPE // for check gmp result
{
    None,
    S_or_T,
};

istream &operator>>(istream &in, POS &pos)
{
    in >> pos.x >> pos.y;
    return in;
}

struct GRID
{
    GRIDTYPE type = None; // for check result
    int vCapacity;        // bottomEdge
    int hCapacity;        // leftEdge
    int vUsage = 0;
    int hUsage = 0;
    double l1cost = 0;
    double l2cost = 0;
};

class NET
{
public:
    NET() {};
    POS s;
    POS t;
    int id;
    vector<POS> Route;
    void setNet(int id, POS pos)
    {
        this->id = id;
        s.x = pos.x;
        s.y = pos.y;
    }
    void setT(POS pos)
    {
        t.x = pos.x;
        t.y = pos.y;
    }
    void printNET(ostream &outfile);
};

//==========================================================
//             for A* Algorithm
//==========================================================
struct Node
{
    int x, y;
    double g, h, f;
    Node *parent; // fo reconstruct path
    Node(int x, int y, Node *parent = nullptr) : x(x), y(y), g(numeric_limits<float>::max()), h(0), f(0), parent(parent) {}
};

inline int heuristic(Node *a, Node *b)
{
    return 0;
    // return abs(a->x - b->x) + abs(a->y - b->y);
}

vector<POS> reconstruct_path(Node *node)
{
    vector<POS> path;
    while (node)
    {
        POS temp;
        temp.x = node->x;
        temp.y = node->y;
        path.push_back(temp);
        node = node->parent;
    }
    reverse(path.begin(), path.end());
    return path;
}

class GlobalRoute
{
public:
    void readGMP(ifstream &infile);
    void readGCL(ifstream &infile);
    void readCST(ifstream &infile);
    void setGMP();
    void printGMP();
    void printNETs(ostream &outfile);
    vector<Node *> get_neighbors(Node *node);
    void a_star(NET &net);
    void a_star_all();
    double distance(Node *a, Node *b) { return abs(a->x - b->x) * gridW + abs(a->y - b->y) * gridH; }

private:
    int gridW, gridH; // single Grid Width and Grid Height

    // below all normalized length
    POS ra; // Routing Area
    int raW, raH;
    POS c1, c2;   // Chip 1, Chip 2
    int c1W, c1H; // Chip 1
    int c2W, c2H; // Chip 2

    // for cost function
    double alpha, beta, gamma, delta, via;

    vector<vector<GRID>> gmp; // Grid Map
    vector<NET> nets;
};

void GlobalRoute::readGMP(ifstream &infile)
{
    string str;
    int id;
    POS pos;

    infile >> str; // .ra
    infile >> ra >> raW >> raH;
    infile >> str; // .g
    infile >> gridW >> gridH;
    infile >> str; // .c1
    infile >> c1 >> c1W >> c1H;
    raW /= gridW; // normalize
    raH /= gridH;
    c1.x /= gridW; // normalize
    c1.y /= gridH;
    c1W /= gridW;
    c1H /= gridH;
    infile >> str; // .b
    while (infile >> str)
    {
        if (str == ".c")
        {
            break;
        }
        else
        {
            infile >> pos;
            pos.x = pos.x / gridW + c1.x; // normalize
            pos.y = pos.y / gridH + c1.y;
            id = stoi(str);
            NET net;
            net.setNet(id, pos);
            nets.push_back(net);
        }
    }
    infile >> c2 >> c2W >> c2H;
    c2.x /= gridW; // normalize
    c2.y /= gridH;
    c2W /= gridW;
    c2H /= gridH;
    infile >> str; // .b
    while (infile >> str)
    {
        infile >> pos;
        pos.x = pos.x / gridW + c2.x; // normalize
        pos.y = pos.y / gridH + c2.y;
        id = stoi(str);
        for (auto &net : nets)
        {
            if (net.id == id)
            {
                net.setT(pos);
                break;
            }
        }
    }
    setGMP();
}

void GlobalRoute::readGCL(ifstream &infile)
{
    string str;
    infile >> str;                // .ec
    for (int j = 0; j < raH; j++) // fast scan order
    {
        for (int i = 0; i < raW; i++)
        {
            infile >> gmp[i][j].hCapacity >> gmp[i][j].vCapacity;
        }
    }
}

void GlobalRoute::readCST(ifstream &infile)
{
    string str;
    infile >> str; // .alpha
    infile >> alpha;
    infile >> str; // .beta
    infile >> beta;
    infile >> str; // .gamma
    infile >> gamma;
    infile >> str; // .delta
    infile >> delta;
    infile >> str; // .v
    infile >> via;
    infile >> str;                // .l
    for (int j = 0; j < raH; j++) // fast scan order
    {
        for (int i = 0; i < raW; i++)
        {
            infile >> gmp[i][j].l1cost;
        }
    }
    infile >> str;                // .l
    for (int j = 0; j < raH; j++) // fast scan order
    {
        for (int i = 0; i < raW; i++)
        {
            infile >> gmp[i][j].l2cost;
        }
    }
}

void GlobalRoute::setGMP()
{
    gmp.resize(raW);
    for (int i = 0; i < raW; i++)
    {
        gmp[i].resize(raH);
    }
    for (int i = 0; i < raW; i++)
    {
        for (int j = 0; j < raH; j++)
        {
            gmp[i][j].vCapacity = 0;
            gmp[i][j].hCapacity = 0;
            gmp[i][j].type = None;
        }
    }
    for (auto &net : nets)
    {
        gmp[net.s.x][net.s.y].type = S_or_T;
        gmp[net.t.x][net.t.y].type = S_or_T;
    }
}

void GlobalRoute::printGMP()
{
    for (int i = 0; i < raH; i++)
    {
        for (int j = 0; j < raW; j++)
        {
            if (gmp[j][raH - i - 1].type == None)
                cout << "- ";
            else
                cout << "s ";
        }
        cout << endl;
    }
}

void GlobalRoute::printNETs(ostream &outfile)
{
    for (auto &net : nets)
    {
        net.printNET(outfile);
    }
}

vector<Node *> GlobalRoute::get_neighbors(Node *node)
{
    vector<Node *> neighbors;
    int dx[] = {0, 1, 0, -1};
    int dy[] = {1, 0, -1, 0};

    for (int i = 0; i < 4; ++i)
    {
        int nx = node->x + dx[i];
        int ny = node->y + dy[i];

        if (nx >= 0 && nx < raW && ny >= 0 && ny < raH)
        {
            neighbors.push_back(new Node(nx, ny, node));
        }
    }

    return neighbors;
}

void GlobalRoute::a_star(NET &net)
{
    Node *start = new Node(net.s.x, net.s.y);
    Node *goal = new Node(net.t.x, net.t.y);
    struct CompareNodes {
        bool operator()(Node *a, Node *b) {
            return a->f > b->f;
        }
    };
    priority_queue<Node *, vector<Node *>, CompareNodes> open_set;
    vector<vector<bool>> closed_set(raW, vector<bool>(raH, false));
    start->g = 0;
    start->h = heuristic(start, goal);
    start->f = start->g + start->h;
    open_set.push(start);

    while (!open_set.empty())
    {
        Node *current = open_set.top();
        open_set.pop();
        if (current->x == goal->x && current->y == goal->y)
        {
            net.Route = reconstruct_path(current);
            return;
        }

        closed_set[current->x][current->y] = true;
        cout << current->x << " " << current->y << endl;
        for (Node *neighbor : get_neighbors(current))
        {
            if (closed_set[neighbor->x][neighbor->y])
            {
                delete neighbor;
                continue;
            }

            // double tentative_g = current->g + ((current->x == neighbor->x) ? gmp[neighbor->x][neighbor->y].l2cost : gmp[neighbor->x][neighbor->y].l1cost);
            double tentative_g = current->g + distance(neighbor, goal);
            cout << "neighbor: " << neighbor->x << " " << neighbor->y << " tentative_g: " << tentative_g << " ";
            if (tentative_g < neighbor->g)
            {
                neighbor->g = tentative_g;
                neighbor->h = heuristic(neighbor, goal);
                neighbor->f = neighbor->g + neighbor->h;
                neighbor->parent = current;
                open_set.push(neighbor);
            }
            else
            {
                delete neighbor;
            }
        }
    }

    net.Route = vector<POS>(); // No path found
}

inline void GlobalRoute::a_star_all()
{
    // for (auto &net : nets)
    // {
    //     a_star(net);
    // }
    a_star(nets[5]);
}

void NET::printNET(ostream &outfile)
{
    cout << "net " << id << " " << s.x << " " << s.y << " " << t.x << " " << t.y << endl;
    if (Route.size() == 0)
    {
        cout << "no route" << endl;
        return;
    }
    int i;
    string name = "n" + to_string(id);
    outfile << name << " " << Route.size() - 2 << endl; // ignore source and target
    outfile << "begin" << endl;
    outfile << Route[0].x << " " << Route[0].y << " "; // source
    POS temp = Route[0];
    for (i = 0; i < Route.size() - 1; i++)
    {
        if ((Route[i + 1].x == temp.x) || (Route[i + 1].y == temp.y)) // corner change both x and y position
        {
            continue;
        }
        temp = Route[i];
        outfile << Route[i].x << " " << Route[i].y << endl;
        outfile << Route[i].x << " " << Route[i].y << " ";
    }
    outfile << Route[Route.size() - 1].x << " " << Route[Route.size() - 1].y << endl; // target
    outfile << "end" << endl;
}

#endif