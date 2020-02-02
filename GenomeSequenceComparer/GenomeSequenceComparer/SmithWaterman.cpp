#include "SmithWaterman.h"

SmithWaterman::SmithWaterman(string s1, string s2, double match, double misMatch, double h, double g)
{
	this->s1 = s1;
	this->s2 = s2;
	this->match = match;
	this->misMatch = misMatch;
	this->h = h;
	this->g = g;
	this->table = new DPTable(s1.length()+1, s2.length()+1);
}

DP_cell* SmithWaterman::GetCalculatedCell(int row, int col)
{
	if (table->IsValidCell(row, col))
	{
		DP_cell* c = table->GetCell(row, col);
		if (c == nullptr)
			c = CalculateCell(row, col);
		return c;
	}
	return nullptr;
}



int SmithWaterman::GetMaxScore(int row, int col, int matchScore)
{
	//deletion
	DP_cell* c = GetCalculatedCell(row - 1, col);
	int max = c->score + this->g;

	//insertion
	c = GetCalculatedCell(row, col - 1);
	if (max < c->score + g)
		max = c->score + g;

	//diag
	c = GetCalculatedCell(row - 1, col - 1);
	if (max < c->score + matchScore)
		max = c->score + matchScore;
	//0
	if (max < 0)
		max = 0;

	return max;

}


DP_cell* SmithWaterman::CalculateCell(int row, int col)
{
	if (row == 0 || col == 0)
		return table->FillInCell(row, col, 0);

	int matchScore = misMatch;
	if (s1[row - 1] == s2[col - 1])
		matchScore = match;

	int maxValue = GetMaxScore(row, col, matchScore);
	return table->FillInCell(row, col, maxValue);


}

bool SmithWaterman::Run()
{
	for (int i = 0; i < s1.length()+1; i++)
	{
		for (int j = 0; j < s2.length()+1; j++)
		{
			CalculateCell(i, j);
		}
	}

	table->PrintTable();
	return false;
}


