// PDA Lab4, implement py pin-yao xu
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <list>

using namespace std;

int main(int argc, char *argv[])
{
    ifstream infile_lg, infile_opt;
    ofstream outfile, out;

    infile_lg.open(argv[1], ios::in);
    infile_opt.open(argv[2], ios::in);
    outfile.open(argv[3], ios::out);
    if (!infile_lg || !infile_opt || !outfile)
    {
        cout << "Error opening files" << endl;
        return 1;
    }

    infile_lg.close();
    infile_opt.close();
    outfile.close();
}

