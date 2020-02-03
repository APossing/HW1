#pragma once
#include <string>
#include "DPTable.h"
using namespace  std;
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
	
	DP_cell* CalculateCell(int row, int col);
	int GetMaxScore(int row, int col, int matchScore);
	DP_cell* GetCalculatedCell(int row, int col);
	list<pair<string, string>> TraceBack(int row, int col, string cur1, string cur2);
	int FindMaxAdjacentSquareValue(int row, int col);
};

