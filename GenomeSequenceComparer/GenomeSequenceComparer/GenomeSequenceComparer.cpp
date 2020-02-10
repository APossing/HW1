// GenomeSequenceComparer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "SmithWaterman.h"
#include "NeedlemanWunsch.h"
#include "AlignmentPrinter.h"

int main()
{

	DPTable t(5, 5);

	//SmithWaterman smith("GATTACA","GCATGCU",1,-1,2,-1);
	//smith.Run();
	//auto q = smith.GetMaxStrings();

	NeedlemanWunsch needle("ACATGCTACACGTATCCGATACCCCGTAACCGATAACGATACACAGACCTCGTACGCTTGCTACAACGTACTCTATAACCGAGAACGATTGACATGCCTCGTACACATGCTACACGTACTCCGAT", "ACATGCGACACTACTCCGATACCCCGTAACCGATAACGATACAGAGACCTCGTACGCTTGCTAATAACCGAGAACGATTGACATTCCTCGTACAGCTACACGTACTCCGAT", 1, -2, -5, -2);
	needle.Run();
	list<Alignment*> qt = needle.GetMaxStrings();
	AlignmentPrinter printer = AlignmentPrinter(qt.front(), "test1.txt");
	//for (auto j : qt)
	//	printer.PrintAlignmentToFile(j);
	int max = 0;
	for (auto j : qt)
	{
		if (j->GetScore(1, -2, -5, -2) > max)
		{
			max = j->GetScore(1, -2, -5, -2);
		}
	}
	for (auto j : qt)
	{
		if (j->GetScore(1, -2, -5, -2) == max)
		{
			printer.PrintAlignmentToFile(j);
		}
	}
	
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
