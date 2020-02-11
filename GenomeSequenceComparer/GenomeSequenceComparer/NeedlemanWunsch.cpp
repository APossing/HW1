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

list<Alignment*> NeedlemanWunsch::GetMaxStrings()
{
	return TraceBack(s1.length(), s2.length(), new Alignment);
}

DP_cell* NeedlemanWunsch::CalculateCell(int row, int col)
{
	if (row == 0)
		return table->FillInCell(row, col, -1 * col, -1 * col, -1 * col);
	else if (col == 0)
		return table->FillInCell(row, col, -1 * row, -1 * row, -1 * row);

	int matchScore = misMatch;
	if (s1[row - 1] == s2[col - 1])
		matchScore = match;

	int subScore = GetMaxSubScore(row, col, matchScore);
	int delScore = GetMaxDeletionScore(row, col);
	int insScore = GetMaxInsertionScore(row, col);

	return table->FillInCell(row, col, subScore, delScore, insScore);
}

int NeedlemanWunsch::GetMaxSubScore(int row, int col, int matchScore)
{
	auto c = GetCalculatedCell(row - 1, col - 1);

	return table->GetCellMax(c) + matchScore;
}
int NeedlemanWunsch::GetMaxDeletionScore(int row, int col)
{
	DP_cell* c = GetCalculatedCell(row - 1, col);
	int max = c->deletionScore + g;

	if (c->insertionScore + h + g > max)
		max = c->insertionScore + h + g;
	if (c->substitutionScore + h + g > max)
		max = c->substitutionScore + h + g;

	return max;
}

int NeedlemanWunsch::GetMaxInsertionScore(int row, int col)
{
	DP_cell* c = GetCalculatedCell(row, col - 1);
	int max = c->deletionScore + h + g;

	if (c->insertionScore + g > max)
		max = c->insertionScore + g;
	if (c->substitutionScore + h + g > max)
		max = c->substitutionScore + h + g;

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

list<DP_cellFull> NeedlemanWunsch::GetMaxAdjacentCells(int row, int col)
{
	list<DP_cellFull> maxAdjacentSquares = list<DP_cellFull>();
	DP_cell* cell = table->GetCell(row, col);
	int max = table->GetCellMax(row, col);
	if (cell->deletionScore == max)
		maxAdjacentSquares.push_back(DP_cellFull(table->GetCell(row - 1, col), row - 1, col, max));
	if (cell->insertionScore == max)
		maxAdjacentSquares.push_back(DP_cellFull(table->GetCell(row, col - 1), row, col - 1, max));
	if (cell->substitutionScore == max)
		maxAdjacentSquares.push_back(DP_cellFull(table->GetCell(row - 1, col - 1), row - 1, col - 1, max));
	return maxAdjacentSquares;
}

list<Alignment*> NeedlemanWunsch::TraceBack(int row, int col, Alignment* alignment)
{
	if (row == 0 && col == 0)
		return { alignment };
	alignment->totalLength++;
	list<DP_cellFull> maxAdjacentSquares = GetMaxAdjacentCells(row, col);
	if (maxAdjacentSquares.empty())
		return { new Alignment() };

	list<Alignment*> returnList = list<Alignment*>();

	//max is 0 and one is diagonal
	if (row > 0 && col > 0 && maxAdjacentSquares.front().max == 0 && table->GetCellMax(table->GetCell(row - 1, col - 1)) == maxAdjacentSquares.front().max)
	{
		alignment->AddS1(s1[row - 1]);
		alignment->AddS2(s2[col - 1]);
		returnList.merge(TraceBack(row - 1, col - 1, alignment->DeepCopy()));
	}
	else
	{
		for (auto fullCell : maxAdjacentSquares)
		{
			if (fullCell.row < row && fullCell.col < col)
			{
				if (s1[row-1] == s2[col-1])
					alignment->matches++;
				else
					alignment->mismatches++;

				alignment->AddS1(s1[row - 1]);
				alignment->AddS2(s2[col - 1]);
			}
			else if (fullCell.row < row)
			{
				if (alignment->s2.length() > 0 && alignment->s2[0] != '-')
					alignment->openingGaps++;
				alignment->gaps++;
				alignment->AddS1(s1[row - 1]);
				alignment->AddS2('-');
			}
			else
			{
				if (alignment->s2.length() > 0 && alignment->s2[0] != '-')
					alignment->openingGaps++;
				alignment->gaps++;
				alignment->AddS1('-');
				alignment->AddS2(s2[col - 1]);
			}
			returnList.merge(TraceBack(fullCell.row, fullCell.col, alignment->DeepCopy()));
		}
	}

	return returnList;
}