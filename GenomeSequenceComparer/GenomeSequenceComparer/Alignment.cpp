#include "Alignment.h"

Alignment::Alignment()
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

Alignment::Alignment(int optimalScore)
{
	this->optimalScore = optimalScore;
	this->matches = 0;
	this->mismatches = 0;
	this->gaps = 0;
	this->openingGaps = 0;
	this->totalLength = 0;
	s1 = "";
	s2 = "";
}

Alignment::Alignment(string s1, string s2, int optimalScore, int matches, int mismatches, int gaps, int openingGaps, int totalLength)
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

void Alignment::AddS1(char c)
{
	s1 = c + s1;
}
void Alignment::AddS2(char c)
{
	s2 = c + s2;
}
