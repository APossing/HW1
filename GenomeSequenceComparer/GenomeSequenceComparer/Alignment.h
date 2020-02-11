#pragma once
#include <String>
using namespace std;
class Alignment
{
public:
	Alignment();
	Alignment(int optimalScore);
	Alignment(string s1, string s2, int optimalScore, int matches, int mismatches, int gaps, int openingGaps, int totalLength);
	Alignment* DeepCopy()
	{
		return new Alignment(s1, s2, optimalScore, matches, mismatches, gaps, openingGaps, totalLength);
	}
	void AddS1(char c); 
	void AddS2(char c);

	double IdentityPercentage()
	{
		return (double)matches / totalLength;
	}
	double GapsPercentage()
	{
		return (double)gaps / totalLength;
	}

	int GetScore(double match, double mismatch, double h, double g)
	{
		return gaps * g + match * matches + mismatch * mismatches + h * openingGaps + gaps * g;
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

