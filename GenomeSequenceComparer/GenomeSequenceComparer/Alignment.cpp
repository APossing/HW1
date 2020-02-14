#include "Alignment.h"

Alignment::Alignment()
{
	this->matches = 0;
	this->mismatches = 0;
	this->gaps = 0;
	this->openingGaps = 0;
	this->totalLength = 0;
	this->s1 = "";
	this->s2 = "";
	this->score = 0;
	this->hasBeenScored = false;
}

Alignment::Alignment(string s1, string s2, int matches, int mismatches, int gaps, int openingGaps, int totalLength, bool hasBeenScored, int score)
{
	this->matches = matches;
	this->mismatches = mismatches;
	this->gaps = gaps;
	this->openingGaps = openingGaps;
	this->totalLength = totalLength;
	this->s1 = s1;
	this->s2 = s2;
	this->hasBeenScored = hasBeenScored;
	this->score = score;
}

Alignment* Alignment::DeepCopy() const
{
	return new Alignment(s1, s2, matches, mismatches, gaps, openingGaps, totalLength, hasBeenScored, score);
}

void Alignment::AddS1(char c)
{
	s1 = c + s1;
}
void Alignment::AddS2(char c)
{
	s2 = c + s2;
}

double Alignment::IdentityPercentage() const
{
	return (double)matches / totalLength;
}

double Alignment::GapsPercentage() const
{
	return (double)gaps / totalLength;
}

int Alignment::GetScore(int match, int mismatch, int h, int g)
{
	if (hasBeenScored)
		return score;
	else
	{
		score = gaps * g + match * matches + mismatch * mismatches + h * openingGaps;
		hasBeenScored = true;
		return score;
	}
}
