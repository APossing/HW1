#pragma once
#include <string>
using namespace std;

enum Direction
{
	up,
	left,
	diag
};


class Alignment
{
public:
	Alignment();
	Alignment(string s1, string s2, int matches, int mismatches, int gaps, int openingGaps, int totalLength, bool hasBeenScored, int score);

	Alignment* DeepCopy() const;
	void AddS1(char c);
	void AddS2(char c);
	double IdentityPercentage() const;
	double GapsPercentage() const;
	int GetScore(int match, int mismatch, int h, int g);
	string s1;
	string s2;
	int matches;
	int mismatches;
	int gaps;
	int openingGaps;
	int totalLength;

private:
	bool hasBeenScored;
	int score;
};

