#pragma once
#include <string.h>
#include "NeedlemanWunsch.h"
#include "FileReader.h"
using namespace std;
#include <string>
#include <fstream>
class AlignmentPrinter
{
public:
	AlignmentPrinter(Alignment* a, string fileName);
	void PrintAlignmentToFile();
	void PrintAlignmentToFile(Alignment* a, Parameters params, MultiSequenceFasta sequenceInfo);

private:
	Alignment* alignment;
	string fileName;
	fstream file;
};

