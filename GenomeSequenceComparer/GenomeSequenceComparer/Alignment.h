#pragma once
#include <String>
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
	Alignment(int optimalScore);
	Alignment(string s1, string s2, int optimalScore, int matches, int mismatches, int gaps, int openingGaps, int totalLength, bool hasBeenScored, int score);
	Alignment* DeepCopy() const
	{
		return new Alignment(s1, s2, optimalScore, matches, mismatches, gaps, openingGaps, totalLength, hasBeenScored, score);
	}
	void AddS1(char c);
	void AddS2(char c);

	double IdentityPercentage() const
	{
		return (double)matches / totalLength;
	}
	double GapsPercentage() const
	{
		return (double)gaps / totalLength;
	}

	int GetScore(int match, int mismatch, int h, int g)
	{
		if (hasBeenScored)
			return score;
		else
		{
			hasBeenScored = true;
			score = gaps * g + match * matches + mismatch * mismatches + h * openingGaps + gaps * g;
			return score;
		}
	}
	string s1;
	string s2;
	int optimalScore;
	int matches;
	int mismatches;
	int gaps;
	int openingGaps;
	int totalLength;

private:
	bool hasBeenScored;
	int score;
};

