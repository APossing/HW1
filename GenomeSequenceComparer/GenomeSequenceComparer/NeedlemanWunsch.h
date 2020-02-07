#pragma once
#include <string>
#include "DPTable.h"
using namespace  std;

enum Direction {
	up,
	left,
	diagonal,
	none
};

class NeedlemanWunsch
{
public:
	NeedlemanWunsch(string s1, string s2, double match, double misMatch, double h, double g);
	bool Run();
	list<pair<string, string>> GetMaxStrings();

private:
	string s1;
	string s2;
	double match;
	double misMatch;
	double h;
	double g;
	DPTable* table;
	Direction TracebackPrevDirection;

	DP_cell* CalculateCell(int row, int col);
	int GetMaxSubScore(int row, int col, int matchScore);
	int GetMaxDeletionScore(int row, int col);
	int GetMaxInsertionScore(int row, int col);
	DP_cell* GetRealCellScores(int row, int col, Direction importantDirection);
	DP_cell* GetCalculatedCell(int row, int col);
	list<pair<string, string>> TraceBack(int row, int col, string cur1, string cur2);
	list<DP_cellFull> FindMaxAdjacentSquares(int row, int col);
};

