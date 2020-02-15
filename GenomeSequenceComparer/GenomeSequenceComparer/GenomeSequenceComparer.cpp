// GenomeSequenceComparer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "AlignmentPrinter.h"
#include "OptimalAlignmentLib.h"
#include "FileReader.h"
int main(int argc, char* argv[])
{
	if (argc < 3)
		cout << "Please add parameters... <input file containing both s1 and s2> <0: global, 1: local> <optional: path to parameters config file>";
	else
	{
		bool isGlobal = true;
		string fileName;
		
		fileName = argv[1];
		if (argv[2][0] == '1')
			isGlobal = false;

		string parametersFileName;
		if (argc > 3)
			parametersFileName = argv[3];
		else
			parametersFileName = "parameters.config";

		Parameters params = FileReader::ReadParameters(parametersFileName);

		MultiSequenceFasta sequenceFileInfo = FileReader::ReadFile(fileName);

		OptimalAlignment optimLib = OptimalAlignment(sequenceFileInfo.s1, sequenceFileInfo.s2, params.match, params.mismatch, params.h, params.g);

		if (isGlobal)
		{
			cout << "Running NeedlemanWunsch" << endl;
			optimLib.RunNeedlemanWunsch();
			Alignment* optimalAlignment = optimLib.GetGlobalMaxStrings();
			AlignmentPrinter::PrintAlignmentToFile(optimalAlignment, params, sequenceFileInfo, "output.txt");
		}
		else
		{
			cout << "Running SmithWaterman" << endl;
			optimLib.RunSmithWaterman();
			list<Alignment*> optimalAlignments = optimLib.GetLocalMaxStrings();
			for (auto alignment : optimalAlignments)
			{
				AlignmentPrinter::PrintAlignmentToFile(alignment, params, sequenceFileInfo, "output.txt");
			}
		}
	}
	return 0;
}
