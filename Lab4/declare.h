#ifndef DECLARE_H
#define DECLARE_H
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
using namespace std;

struct POS // position
{
    int x;
    int y;
};

enum GRIDTYPE
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

class GlobalRoute
{
public:
    void readGMP(ifstream &infile);
    void readGCL(ifstream &infile);
    void readCST(ifstream &infile);
    void setGMP();
    void printGMP();

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
    for (int j = 0; j < raH; j++) // fast scan order
    {
        for (int i = 0; i < raW; i++)
        {

        }
    }
}

void NET::printNET(ostream &outfile)
{
    // int i;
    // string name = "n" + to_string(id);
    // outfile << name << " " << Route.size() - 2 << endl; // ignore source and target
    // outfile << "begin" << endl;
    // outfile << Route[0].x << " " << Route[0].y << " "; // source
    // POS temp = Route[0];
    // for (i = 0; i < Route.size() - 1; i++)
    // {
    //     if ((Route[i + 1].x == temp.x) || (Route[i + 1].y == temp.y)) // corner change both x and y position
    //     {
    //         continue;
    //     }
    //     temp = Route[i];
    //     outfile << Route[i].x << " " << Route[i].y << endl;
    //     outfile << Route[i].x << " " << Route[i].y << " ";
    // }
    // outfile << Route[Route.size() - 1].x << " " << Route[Route.size() - 1].y << endl; // target
    // outfile << "end" << endl;
}

#endif