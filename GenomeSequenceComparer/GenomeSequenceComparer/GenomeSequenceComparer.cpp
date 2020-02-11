// GenomeSequenceComparer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "SmithWaterman.h"
#include "NeedlemanWunsch.h"
#include "AlignmentPrinter.h"
#include "OptimalAlignmentLib.h"
#include "FileReader.h"
int main(int argc, char* argv[])
{
	bool isGlobal = true;
	string fileName;
	fileName = argv[1];
	if (argv[2][0] == 1)
		isGlobal = false;

	string parametersFileName = "";
	if (argc > 3)
		parametersFileName = argv[3];

	Parameters params = FileReader::ReadParameters("parameters.config.txt");


	MultiSequenceFasta sequenceFileInfo = FileReader::ReadFile("Opsin1_colorblindness_gene.fasta.txt");

	OptimalAlignment optim = OptimalAlignment(sequenceFileInfo.s1, sequenceFileInfo.s2, params.match, params.mismatch, params.h, params.g);

	if (isGlobal)
	{
		optim.RunNeedlemanWunsch();
		list<Alignment*> qt = optim.GetGlobalMaxStrings();
		AlignmentPrinter printer = AlignmentPrinter(qt.front(), "test1.txt");
		int max = qt.front()->GetScore(params.match, params.mismatch, params.h, params.g);
		for (auto j : qt)
		{
			if (j->GetScore(params.match, params.mismatch, params.h, params.g) > max)
			{
				max = j->GetScore(params.match, params.mismatch, params.h, params.g);
			}
		}
		for (auto j : qt)
		{
			if (j->GetScore(params.match, params.mismatch, params.h, params.g) == max)
			{
				printer.PrintAlignmentToFile(j, params, sequenceFileInfo);
			}
		}
	}
	else
	{
		optim.RunSmithWaterman();
		list<list<Alignment*>> qt = optim.GetLocalMaxStrings();
		AlignmentPrinter printer = AlignmentPrinter(qt.front().front(), "test1.txt");
		int max = 0;
		for (auto k : qt)
		{
			for (auto j : k)
			{
				if (j->GetScore(1, -2, -5, -2) > max)
				{
					max = j->GetScore(1, -2, -5, -2);
				}
			}
		}
		for (auto k : qt)
		{
			for (auto j : k)
			{
				if (j->GetScore(1, -2, -5, -2) == max)
				{
					printer.PrintAlignmentToFile(j, params, sequenceFileInfo);
				}
			}
		}
	}

}
