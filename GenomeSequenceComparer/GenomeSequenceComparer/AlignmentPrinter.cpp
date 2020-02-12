#include "AlignmentPrinter.h"

void AlignmentPrinter::PrintAlignmentToFile(Alignment* alignment, Parameters params, MultiSequenceFasta sequenceInfo, string fileName)
{
	fstream file = fstream(fileName);
	
	file << "Scores: match = " << params.match << ", mismatch = " << params.mismatch << ", h = " << params.h << ", g = " << params.g << endl << endl;
	file << "Sequence 1 = \"" << sequenceInfo.name1 << "\", length = " << sequenceInfo.s1.length() << " characters" << endl;
	file << "Sequence 2 = \"" << sequenceInfo.name2 << "\", length = " << sequenceInfo.s2.length() << " characters" << endl << endl;

	char prev = ' ';

	int s1Count = 1;
	int s2Count = 1;

	bool s2Big = false;
	bool s1Big = false;
	bool bothBig = false;

	for (int i = 0; i < alignment->s1.length() / 60 + 1; i++)
	{
		if (s2Count >= 1000 && s1Count < 1000)
		{
			s2Big = true;
			s1Big = false;
		}
		else if (s2Count < 1000 && s1Count >= 1000)
		{
			s2Big = false;
			s1Big = true;
		}
		else if (s2Count >= 1000 && s1Count >= 1000)
		{
			s2Big = false;
			s1Big = false;
			bothBig = true;
		}

		
		if (s2Big)
			file << "s1\t" << s1Count << "\t\t";
		else if (s1Big)
			file << "s1\t" << s1Count << "\t";
		else if (bothBig)
			file << "s1\t" << s1Count << "\t";
		else
			file << "s1\t" << s1Count << "\t\t";
		for (int j = 0; j < 60 && i * 60 + j < alignment->s1.length(); j++)
		{
			file << alignment->s1[i * 60 + j];
			if (alignment->s1[i * 60 + j] != '-')
				s1Count++;
		}
		if (s1Big || s2Big || bothBig )
			file << "\t" << s1Count-1 << endl << "\t\t\t";
		else
			file << "\t" << s1Count-1 << endl << "\t\t\t";

		for (int j = 0; j < 60 && i * 60 + j < alignment->s1.length(); j++)
		{
			if (alignment->s1[i * 60 + j] == alignment->s2[i * 60 + j] && alignment->s1[i * 60 + j] != '-')
				file << '|';
			else
				file << ' ';
		}
		file << endl;
		if (s1Big)
			file << "s2\t" << s2Count << "\t\t";
		else if (s2Big)
			file << "s2\t" << s2Count << "\t";
		else if (bothBig)
			file << "s2\t" << s2Count << "\t";
		else
			file << "s2\t" << s2Count << "\t\t";


		for (int j = 0; j < 60 && i * 60 + j < alignment->s2.length(); j++)
		{
			file << alignment->s2[i * 60 + j];
			if (alignment->s2[i * 60 + j] != '-')
				s2Count++;
		}
		file << "\t" << s2Count-1 << endl << endl;
	}
	file << endl << endl << endl;

	file << "report:" << endl << endl;
	file << "global optimal score = " << alignment->optimalScore << endl << endl;
	file << "number of: matches = " << alignment->matches << ", mismatches = " << alignment->mismatches << ", gaps = " << alignment->gaps << ", opening gaps = " << alignment->openingGaps << endl;
	file << "identities = " << alignment->matches << "/" <<alignment->s1.length()<< "(" << (int)(alignment->IdentityPercentage()*100)<< "%), gaps = " << alignment->gaps << "/" << alignment->s1.length() << "(" << (int)(alignment->GapsPercentage() * 100) << "%)" <<endl;

}
