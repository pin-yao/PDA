// PDA Lab4, implement py pin-yao xu
#include <iostream>
#include <fstream>
#include "declare.h"

using namespace std;

int main(int argc, char *argv[])
{
    ifstream infile_gmp, infile_gcl, infile_cst;
    ofstream outfile;
    infile_gmp.open(argv[1], ios::in);
    infile_gcl.open(argv[2], ios::in);
    infile_cst.open(argv[3], ios::in);
    outfile.open(argv[4], ios::out);
    if (!infile_gmp || !infile_gcl || !infile_cst || !outfile)
    {
        cout << "Error opening files" << endl;
        return 1;
    }

    GlobalRoute gr;
    gr.readGMP(infile_gmp);
    infile_gmp.close();
    gr.readGCL(infile_gcl);
    infile_gcl.close();
    gr.readCST(infile_cst);
    infile_cst.close();
    gr.printGMP();

    outfile.close();
}
