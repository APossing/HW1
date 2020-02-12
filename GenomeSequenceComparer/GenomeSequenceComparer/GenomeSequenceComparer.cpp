// GenomeSequenceComparer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
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
		Alignment* optimalAlignment = optim.GetGlobalMaxStrings();
		AlignmentPrinter::PrintAlignmentToFile(optimalAlignment, params, sequenceFileInfo, "test1.txt");
	}
	else
	{
		optim.RunSmithWaterman();
		list<Alignment*> optimalAlignments = optim.GetLocalMaxStrings();
		int max = 0;
		for (auto alignment : optimalAlignments)
		{
			AlignmentPrinter::PrintAlignmentToFile(alignment, params, sequenceFileInfo, "test1.txt");

		}
	}

}
