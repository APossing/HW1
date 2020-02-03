#include "NeedlemanWunsch.h"

NeedlemanWunsch::NeedlemanWunsch(string s1, string s2, double match, double misMatch, double h, double g)
{
	this->s1 = s1;
	this->s2 = s2;
	this->match = match;
	this->misMatch = misMatch;
	this->h = h;
	this->g = g;
	this->table = new DPTable(s1.length() + 1, s2.length() + 1);
}

bool NeedlemanWunsch::Run()
{
	for (int i = 0; i < s1.length() + 1; i++)
	{
		for (int j = 0; j < s2.length() + 1; j++)
		{
			CalculateCell(i, j);
		}
	}

	table->PrintTable();
	return true;
}

list<pair<string, string>> NeedlemanWunsch::GetMaxStrings()
{
	return TraceBack(s1.length(), s2.length(), "", "");
}

DP_cell* NeedlemanWunsch::CalculateCell(int row, int col)
{
	if (row == 0)
		return table->FillInCell(row, col, -1 * col);
	else if (col == 0)
		return table->FillInCell(row, col, -1 * row);

	int matchScore = misMatch;
	if (s1[row - 1] == s2[col - 1])
		matchScore = match;

	int maxValue = GetMaxScore(row, col, matchScore);
	return table->FillInCell(row, col, maxValue);
}

int NeedlemanWunsch::GetMaxScore(int row, int col, int matchScore)
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

	return max;
}

DP_cell* NeedlemanWunsch::GetCalculatedCell(int row, int col)
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

list<pair<string, string>> NeedlemanWunsch::TraceBack(int row, int col, string cur1, string cur2)
{
	if (row == 0 && col == 0)
		return { {cur1,cur2} };

	int max = FindMaxAdjacentSquareValue(row, col);
	list<pair<string, string>> returnList = list<pair<string, string>>();

	//max is 0 and one its diagonal
	if (max == 0 && table->GetCell(row - 1, col - 1)->score == max)
	{
		returnList.merge(TraceBack(row - 1, col - 1, s1[row - 1] + cur1, s2[col - 1] + cur2));
	}
	else
	{
		//insertion (up)
		if (table->GetCell(row - 1, col)->score == max)
		{
			returnList.merge(TraceBack(row - 1, col, s1[row - 1] + cur1, "-" + cur2));
		}

		//deletion (left)
		if (table->GetCell(row, col - 1)->score == max)
		{
			returnList.merge(TraceBack(row, col - 1, "-" + cur1, s2[col - 1] + cur2));
		}

		//direct (diag)
		if (table->GetCell(row - 1, col - 1)->score == max)
		{
			returnList.merge(TraceBack(row - 1, col - 1, s1[row - 1] + cur1, s2[col - 1] + cur2));
		}
	}

	return returnList;
}

int NeedlemanWunsch::FindMaxAdjacentSquareValue(int row, int col)
{
	int max = table->GetCell(row - 1, col)->score;
	int tempVal = table->GetCell(row, col - 1)->score;
	if (tempVal > max)
		max = tempVal;
	tempVal = table->GetCell(row - 1, col - 1)->score;
	if (tempVal > max)
		max = tempVal;

	return max;
}
