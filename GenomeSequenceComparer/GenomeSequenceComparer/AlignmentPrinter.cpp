#include "AlignmentPrinter.h"

AlignmentPrinter::AlignmentPrinter(Alignment *a, string fileName)
{
	this->alignment = a;
	this->fileName = fileName;
	
	this->file = fstream();
	this->file.open(this->fileName);
}

void AlignmentPrinter::PrintAlignmentToFile()
{
	file << this->alignment->s1 << endl;
	char prev = ' ';
	for (int i = 0; i < this->alignment->s1.length(); i++)
	{
		if (this->alignment->s1[i] == this->alignment->s2[i] && this->alignment->s1[i] != '-')
			file << '|';
		else
			file << ' ';
	}
	file << endl;
	file << this->alignment->s2 << endl;
}
void AlignmentPrinter::PrintAlignmentToFile(Alignment* a, Parameters params, MultiSequenceFasta sequenceInfo)
{
	file << "Scores: Match = " << params.match << ", mismatch = " << params.mismatch << ", h = " << params.h << ", g = " << params.g << endl;
	file << "Sequence 1 = \""<< sequenceInfo.name1 <<"\", length = " << sequenceInfo.s1.length() << " characters" << endl;
	file << "Sequence 1 = \"" << sequenceInfo.name2 <<"\", length = " << sequenceInfo.s2.length() << " characters" << endl << endl;

	char prev = ' ';

	int s1Count = 1;
	int s2Count = 1;

	for (int i = 0; i < a->s1.length() / 60 + 1; i++)
	{
		file << "s1\t" << s1Count << "\t";
		for (int j = 0; j < 60 && i * 60 + j < a->s1.length(); j++)
		{
			file << a->s1[i * 60 + j];
			if (a->s1[i * 60 + j] != '-')
				s1Count++;
		}
		file << "\t" << s1Count << endl << "\t\t";

		for (int j = 0; j < 60 && i * 60 + j < a->s1.length(); j++)
		{
			if (a->s1[j] == a->s2[j] && a->s1[j] != '-')
				file << '|';
			else
				file << ' ';
		}
		file << endl;

		file << "s2\t" << s2Count << "\t";
		for (int j = 0; j < 60 && i * 60 + j < a->s2.length(); j++)
		{
			file << a->s2[i * 60 + j];
			if (a->s2[i * 60 + j] != '-')
				s2Count++;
		}
		file << "\t" << s1Count << endl << endl;
	}
	file << endl;
	file << a->s2 << endl << endl;
}
