#pragma once
#include <String>
#include "Alignment.h"
#include "DPTable.h"
using namespace std;
class OptimalAlignment
{
public:
	OptimalAlignment(string s1, string s2, double match, double misMatch, double h, double g);
	bool RunNeedlemanWunsch();
	bool RunSmithWaterman();
	list<Alignment*> GetGlobalMaxStrings();
private:
	string s1;
	string s2;
	double match;
	double misMatch;
	double h;
	double g;
	DPTable* table;

	void init();
	DP_cell* CalculateCell(int row, int col);
	DP_cell* CalculateCell(int row, int col, int min);
	int GetMaxSubScore(int row, int col, int matchScore);
	int GetMaxDeletionScore(int row, int col);
	int GetMaxInsertionScore(int row, int col);
	DP_cell* GetCalculatedCell(int row, int col);
	list<Alignment*> TraceBack(int row, int col, Alignment* alignment);
	list<DP_cellFull> GetMaxAdjacentCells(int row, int col);
};

