// GenomeSequenceComparer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "SmithWaterman.h"

int main()
{

    DPTable t(5,5);
    DP_cell* d = t.FillInCell(0, 0, 10);
    DP_cell* c = t.FillInCell(0, 10, 10);

    SmithWaterman smith("CATTCAC","CTCGCAGC",1,-1,2,-5);

    smith.Run();

    auto q = smith.GetMaxStrings();

    int b = 5;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view error
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
