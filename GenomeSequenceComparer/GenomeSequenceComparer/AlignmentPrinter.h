#pragma once
#include <string.h>
#include "NeedlemanWunsch.h"
using namespace std;
#include <string>
#include <fstream>
class AlignmentPrinter
{
public:
	AlignmentPrinter(Alignment* a, string fileName);
	void PrintAlignmentToFile();
	void PrintAlignmentToFile(Alignment* a);

private:
	Alignment* alignment;
	string fileName;
	fstream file;
};

