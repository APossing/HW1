#pragma once
#include "FileReader.h"
#include "Alignment.h"
using namespace std;
#include <string>
#include <fstream>
class AlignmentPrinter
{
public:
	static void PrintAlignmentToFile(Alignment* alignment, Parameters params, MultiSequenceFasta sequenceInfo, string fileName);
};

