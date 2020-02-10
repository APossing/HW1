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

struct Alignment
{
public:
	Alignment()
	{
		this->optimalScore = 0;
		this->matches = 0;
		this->mismatches = 0;
		this->gaps = 0;
		this->openingGaps = 0;
		this->totalLength = 0;
		s1 = "";
		s2 = "";
	}
	Alignment(string s1, string s2, int optimalScore, int matches, int mismatches, int gaps, int openingGaps, int totalLength)
	{
		this->optimalScore = optimalScore;
		this->matches = matches;
		this->mismatches = mismatches;
		this->gaps = gaps;
		this->openingGaps = openingGaps;
		this->totalLength = totalLength;
		this->s1 = s1;
		this->s2 = s2;
	}
	Alignment* DeepCopy()
	{
		return new Alignment(s1, s2, optimalScore, matches, mismatches, gaps, openingGaps, totalLength);
	}
	void AddS1(char c)
	{
		s1 = c + s1;
	}
	void AddS2(char c)
	{
		s2 = c + s2;
	}

	double IdentityPercentage()
	{
		return matches / totalLength;
	}
	double GapsPercentage()
	{
		return gaps / totalLength;
	}
	string s1;
	string s2;
	int optimalScore;
	int matches;
	int mismatches;
	int gaps;
	int openingGaps;
	int totalLength;
};

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
	Direction TracebackPrevDirection;

	DP_cell* CalculateCell(int row, int col);
	int GetMaxSubScore(int row, int col, int matchScore);
	int GetMaxDeletionScore(int row, int col);
	int GetMaxInsertionScore(int row, int col);
	DP_cell* GetRealCellScores(int row, int col, Direction importantDirection);
	DP_cell* GetCalculatedCell(int row, int col);
	list<Alignment*> TraceBack(int row, int col, Alignment* alignment);
	list<DP_cellFull> FindMaxAdjacentSquares(int row, int col);
};

