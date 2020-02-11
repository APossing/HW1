#include "Alignment.h"

Alignment::Alignment()
{
	this->optimalScore = 0;
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

Alignment::Alignment(int optimalScore)
{
	this->optimalScore = optimalScore;
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

Alignment::Alignment(string s1, string s2, int optimalScore, int matches, int mismatches, int gaps, int openingGaps, int totalLength, bool hasBeenScored, int score)
{
	this->optimalScore = optimalScore;
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

void Alignment::AddS1(char c)
{
	s1 = c + s1;
}
void Alignment::AddS2(char c)
{
	s2 = c + s2;
}
