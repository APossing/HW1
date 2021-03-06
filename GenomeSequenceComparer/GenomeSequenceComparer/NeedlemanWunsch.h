#pragma once
#include <string>
#include "DPTable.h"
#include "Alignment.h"
using namespace  std;

class NeedlemanWunsch
{
public:
	NeedlemanWunsch(string s1, string s2, double match, double misMatch, double h, double g);
	bool Run();
	list<Alignment*> GetMaxStrings();

private:
	string s1;
	string s2;
	double match;
	double misMatch;
	double h;
	double g;
	DPTable* table;

	DP_cell* CalculateCell(int row, int col);
	int GetMaxSubScore(int row, int col, int matchScore);
	int GetMaxDeletionScore(int row, int col);
	int GetMaxInsertionScore(int row, int col);
	DP_cell* GetCalculatedCell(int row, int col);
	list<Alignment*> TraceBack(int row, int col, Alignment* alignment);
	list<DP_cellFull> GetMaxAdjacentCells(int row, int col);
};

