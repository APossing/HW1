// GenomeSequenceComparer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "AlignmentPrinter.h"
#include "OptimalAlignmentLib.h"
#include "FileReader.h"
#include "ProgramSettings.h"

int main(int argc, char* argv[])
{
	ProgramSettings* settings = ProgramSettings::GetProgramSettings(argc, argv);
	if (settings == nullptr)
		return 0;
	Parameters params = FileReader::ReadParameters(settings->parametersFileName);
	
	MultiSequenceFasta sequenceFileInfo = FileReader::ReadFile(settings->inFileName);
	
	OptimalAlignment optimLib = OptimalAlignment(sequenceFileInfo.s1, sequenceFileInfo.s2, params.match, params.mismatch, params.h, params.g);

	if (settings->isGlobal)
	{
		if (settings->isParallel)
		{
			cout << "Running NeedlemanWunsch parallel with " << settings->threadCount << " threads" << endl;
			optimLib.RunNeedlemanWunschParallel(settings->threadCount);
			Alignment* optimalAlignment = optimLib.GetGlobalMaxStrings();
			AlignmentPrinter::PrintAlignmentToFile(optimalAlignment, params, sequenceFileInfo, "output.txt");
		}
		else
		{
			cout << "Running NeedlemanWunsch" << endl;

			optimLib.RunNeedlemanWunsch();
			Alignment* optimalAlignment = optimLib.GetGlobalMaxStrings();
			AlignmentPrinter::PrintAlignmentToFile(optimalAlignment, params, sequenceFileInfo, "output.txt");
		}
	}
	else
	{
		if (settings->isParallel)
		{
			cout << "Running SmithWaterman parallel with " << settings->threadCount << " threads" << endl;
			optimLib.RunSmithWatermanParallel(settings->threadCount);
			list<Alignment*> optimalAlignments = optimLib.GetLocalMaxStrings();
			for (auto alignment : optimalAlignments)
			{
				AlignmentPrinter::PrintAlignmentToFile(alignment, params, sequenceFileInfo, "output.txt");
			}
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
	return 1;
}
