#pragma once
#include "Alignment.h"
#include "DPTable.h"
using namespace std;
class OptimalAlignment
{
public:
	OptimalAlignment(string s1, string s2, int match, int misMatch, int h, int g);
	bool RunNeedlemanWunsch();
	bool RunNeedlemanWunschParallel(int numThreads);
	bool RunSmithWatermanParallel(int numThreads);
	bool RunSmithWaterman();
	Alignment* GetGlobalMaxStrings();
	list<Alignment*> GetLocalMaxStrings();
private:
	string s1;
	string s2;
	int match;
	int misMatch;
	int h;
	int g;
	DPTable* table;

	void init();
	DP_cell* CalculateCell(int row, int col);
	DP_cell* CalculateCell(int row, int col, int min);
	int GetMaxSubScore(int row, int col);
	int GetMaxDeletionScore(int row, int col);
	int GetMaxInsertionScore(int row, int col);
	DP_cell* GetCalculatedCell(int row, int col);
	Alignment* TraceBackGlobal(int row, int col, Alignment* alignment);
	Alignment* TraceBackLocal(int row, int col, Alignment* alignment);
	list<DP_cellFull> GetMaxAdjacentCells(int row, int col, Direction prevDirection) const;
	void AddPointsToAlignment(int curRow, int curCol, Alignment* alignment);
};

