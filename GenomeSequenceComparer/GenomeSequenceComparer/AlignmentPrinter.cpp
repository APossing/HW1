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
void AlignmentPrinter::PrintAlignmentToFile(Alignment *a)
{
	file << a->s1 << endl;
	char prev = ' ';
	for (int i = 0; i < a->s1.length(); i++)
	{
		if (a->s1[i] == a->s2[i] && a->s1[i] != '-')
			file << '|';
		else
			file << ' ';
	}
	file << endl;
	file << a->s2 << endl << endl;
}
