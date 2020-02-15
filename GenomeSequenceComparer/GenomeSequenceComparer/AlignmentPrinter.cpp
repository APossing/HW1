#include "AlignmentPrinter.h"
#include <iostream>

void AlignmentPrinter::PrintAlignmentToFile(Alignment* alignment, Parameters params, MultiSequenceFasta sequenceInfo, string fileName)
{
	//fstream file = fstream(fileName, std::fstream::out | std::fstream::trunc);
	
	cout << "Scores: match = " << params.match << ", mismatch = " << params.mismatch << ", h = " << params.h << ", g = " << params.g << endl << endl;
	cout << "Sequence 1 = \"" << sequenceInfo.name1 << "\", length = " << sequenceInfo.s1.length() << " characters" << endl;
	cout << "Sequence 2 = \"" << sequenceInfo.name2 << "\", length = " << sequenceInfo.s2.length() << " characters" << endl << endl;

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
			cout << "s1\t" << s1Count << "\t";
		else if (s1Big)
			cout << "s1\t" << s1Count << "\t";
		else if (bothBig)
			cout << "s1\t" << s1Count << "\t";
		else
			cout << "s1\t" << s1Count << "\t";
		for (int j = 0; j < 60 && i * 60 + j < alignment->s1.length(); j++)
		{
			cout << alignment->s1[i * 60 + j];
			if (alignment->s1[i * 60 + j] != '-')
				s1Count++;
		}
		if (s1Big || s2Big || bothBig )
			cout << "\t" << s1Count-1 << endl << "\t\t";
		else
			cout << "\t" << s1Count-1 << endl << "\t\t";

		for (int j = 0; j < 60 && i * 60 + j < alignment->s1.length(); j++)
		{
			if (alignment->s1[i * 60 + j] == alignment->s2[i * 60 + j] && alignment->s1[i * 60 + j] != '-')
				cout << '|';
			else
				cout << ' ';
		}
		cout << endl;
		if (s1Big)
			cout << "s2\t" << s2Count << "\t";
		else if (s2Big)
			cout << "s2\t" << s2Count << "\t";
		else if (bothBig)
			cout << "s2\t" << s2Count << "\t";
		else
			cout << "s2\t" << s2Count << "\t";


		for (int j = 0; j < 60 && i * 60 + j < alignment->s2.length(); j++)
		{
			cout << alignment->s2[i * 60 + j];
			if (alignment->s2[i * 60 + j] != '-')
				s2Count++;
		}
		cout << "\t" << s2Count-1 << endl << endl;
	}
	cout << endl << endl << endl;

	cout << "report:" << endl << endl;
	cout << "global optimal score = " << alignment->GetScore(params.match, params.mismatch, params.h, params.g) << endl << endl;
	cout << "number of: matches = " << alignment->matches << ", mismatches = " << alignment->mismatches << ", gaps = " << alignment->gaps << ", opening gaps = " << alignment->openingGaps << endl;
	cout << "identities = " << alignment->matches << "/" <<alignment->s1.length()<< "(" << (int)(alignment->IdentityPercentage()*100)<< "%), gaps = " << alignment->gaps << "/" << alignment->s1.length() << "(" << (int)(alignment->GapsPercentage() * 100) << "%)" <<endl;
}
