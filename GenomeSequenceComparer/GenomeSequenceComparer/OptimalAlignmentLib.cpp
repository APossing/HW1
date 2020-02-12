#include "OptimalAlignmentLib.h"
#include <vector>
#include <omp.h>

OptimalAlignment::OptimalAlignment(string s1, string s2, double match, double misMatch, double h, double g)
{
	this->s1 = s1;
	this->s2 = s2;
	this->match = match;
	this->misMatch = misMatch;
	this->h = h;
	this->g = g;
	this->table = nullptr;
}
void OptimalAlignment::init()
{
	if (this->table != nullptr)
		delete this->table;
	this->table = new DPTable(s1.length() + 1, s2.length() + 1);
}


bool OptimalAlignment::RunNeedlemanWunsch()
{
	init();

	for (int i = 0; i < s1.length() + 1; i++)
		for (int j = 0; j < s2.length() + 1; j++)
			CalculateCell(i, j);

	//	table->PrintTable();
	return true;
}

bool OptimalAlignment::RunSmithWaterman()
{
	init();

	for (int i = 0; i < s1.length() + 1; i++)
		for (int j = 0; j < s2.length() + 1; j++)
			CalculateCell(i, j, 0);

	//	table->PrintTable();
	return true;
}

Alignment* OptimalAlignment::GetGlobalMaxStrings()
{
	return TraceBackGlobal(s1.length(), s2.length(), new Alignment());
}
list<Alignment*> OptimalAlignment::GetLocalMaxStrings()
{
	list<DP_cellFull*> startingLocals = table->GetMaxCells();
	list<Alignment*> returnAlignments = list<Alignment*>();
	for (auto local : startingLocals)
		returnAlignments.push_back(TraceBackLocal(s1.length(), s2.length(), new Alignment));
	return returnAlignments;
}

DP_cell* OptimalAlignment::CalculateCell(int row, int col)
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

DP_cell* OptimalAlignment::CalculateCell(int row, int col, int min)
{
	if (row == 0)
		return table->FillInCell(row, col, -1 * col, -1 * col, -1 * col);
	else if (col == 0)
		return table->FillInCell(row, col, -1 * row, -1 * row, -1 * row);

	int matchScore = misMatch;
	if (s1[row - 1] == s2[col - 1])
		matchScore = match;

	int subScore = GetMaxSubScore(row, col, matchScore);
	if (subScore < min)
		subScore = min;

	int delScore = GetMaxDeletionScore(row, col);
	if (delScore < min)
		delScore = min;

	int insScore = GetMaxInsertionScore(row, col);
	if (insScore < min)
		insScore = min;

	return table->FillInCell(row, col, subScore, delScore, insScore);
}

int OptimalAlignment::GetMaxSubScore(int row, int col, int matchScore)
{
	auto c = GetCalculatedCell(row - 1, col - 1);

	return table->GetCellMax(c) + matchScore;
}
int OptimalAlignment::GetMaxDeletionScore(int row, int col)
{
	DP_cell* c = GetCalculatedCell(row - 1, col);
	int max = c->deletionScore + g;

	if (c->insertionScore + h + g > max)
		max = c->insertionScore + h + g;
	if (c->substitutionScore + h + g > max)
		max = c->substitutionScore + h + g;

	return max;
}

int OptimalAlignment::GetMaxInsertionScore(int row, int col)
{
	DP_cell* c = GetCalculatedCell(row, col - 1);
	int max = c->deletionScore + h + g;

	if (c->insertionScore + g > max)
		max = c->insertionScore + g;
	if (c->substitutionScore + h + g > max)
		max = c->substitutionScore + h + g;

	return max;
}

DP_cell* OptimalAlignment::GetCalculatedCell(int row, int col)
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

list<DP_cellFull> OptimalAlignment::GetMaxAdjacentCells(int row, int col, Direction prevDirection)
{
	list<DP_cellFull> maxAdjacentSquares = list<DP_cellFull>();
	DP_cell* cell = table->GetCell(row, col)->DeepCopy();
	if (prevDirection == Direction::left)
	{
		cell->deletionScore += h;
		cell->substitutionScore += h;
	}
	if (prevDirection == Direction::up)
	{
		cell->insertionScore += h;
		cell->substitutionScore += h;
	}
	int max = table->GetCellMax(cell);

	if (row == 0)
	{
		maxAdjacentSquares.push_back(DP_cellFull(table->GetCell(row, col - 1), row, col - 1, max));
		return maxAdjacentSquares;
	}
	if (col == 0)
	{
		maxAdjacentSquares.push_back(DP_cellFull(table->GetCell(row - 1, col), row - 1, col, max));
		return maxAdjacentSquares;
	}

	if (cell->substitutionScore == max)
		maxAdjacentSquares.push_back(DP_cellFull(table->GetCell(row - 1, col - 1), row - 1, col - 1, max));
	if (cell->deletionScore == max)
		maxAdjacentSquares.push_back(DP_cellFull(table->GetCell(row - 1, col), row - 1, col, max));
	if (cell->insertionScore == max)
		maxAdjacentSquares.push_back(DP_cellFull(table->GetCell(row, col - 1), row, col - 1, max));

	return maxAdjacentSquares;
}

Alignment* OptimalAlignment::TraceBackGlobal(int row, int col, Alignment* alignment)
{
	int curRow = row;
	int curCol = col;
	Alignment* curAlign = alignment;
	Direction prevDirection = Direction::diag;

	do
	{
		//start first
		if (table->GetCellMax(curRow, curCol) > curAlign->optimalScore)
			curAlign->optimalScore = table->GetCellMax(curRow, curCol);

		list<DP_cellFull> maxAdjacentSquares = GetMaxAdjacentCells(curRow, curCol, prevDirection);

		auto fullCell = maxAdjacentSquares.front();
		if (fullCell.row < curRow && fullCell.col < curCol)
		{
			//substitution
			if (s1[curRow - 1] == s2[curCol - 1])
				curAlign->matches++;
			else
				curAlign->mismatches++;

			curAlign->AddS1(s1[curRow - 1]);
			curAlign->AddS2(s2[curCol - 1]);
			prevDirection = Direction::diag;
		}
		else if (fullCell.row < curRow)
		{
			//deletion
			if (curAlign->s2.length() > 0 && curAlign->s2[0] != '-')
				curAlign->openingGaps++;
			curAlign->gaps++;
			curAlign->AddS1(s1[curRow - 1]);
			curAlign->AddS2('-');
			prevDirection = Direction::up;
		}
		else
		{
			//insertion
			if (curAlign->s2.length() > 0 && curAlign->s2[0] != '-')
				curAlign->openingGaps++;
			curAlign->gaps++;
			curAlign->AddS1('-');
			curAlign->AddS2(s2[curCol - 1]);
			prevDirection = Direction::left;
		}
		curRow = fullCell.row;
		curCol = fullCell.col;

	} while (curRow != 0 && curCol != 0);

	return curAlign;
}

Alignment* OptimalAlignment::TraceBackLocal(int row, int col, Alignment* alignment)
{
	int curRow = row;
	int curCol = col;
	Alignment* curAlign = alignment;
	DP_cellFull fullCell(nullptr, 0, 0, 0);
	Direction prevDirection = Direction::diag;

	do
	{
		if (table->GetCellMax(curRow, curCol) > curAlign->optimalScore)
			curAlign->optimalScore = table->GetCellMax(curRow, curCol);

		list<DP_cellFull> maxAdjacentSquares = GetMaxAdjacentCells(curRow, curCol, prevDirection);

		fullCell = maxAdjacentSquares.front();
		if (fullCell.row < curRow && fullCell.col < curCol)
		{
			//substitution
			if (s1[curRow - 1] == s2[curCol - 1])
				curAlign->matches++;
			else
				curAlign->mismatches++;

			curAlign->AddS1(s1[curRow - 1]);
			curAlign->AddS2(s2[curCol - 1]);
			prevDirection = Direction::diag;

		}
		else if (fullCell.row < curRow)
		{
			//deletion
			if (curAlign->s2.length() > 0 && curAlign->s2[0] != '-')
				curAlign->openingGaps++;
			curAlign->gaps++;
			curAlign->AddS1(s1[curRow - 1]);
			curAlign->AddS2('-');
			prevDirection = Direction::up;
		}
		else
		{
			//insertion
			if (curAlign->s2.length() > 0 && curAlign->s2[0] != '-')
				curAlign->openingGaps++;
			curAlign->gaps++;
			curAlign->AddS1('-');
			curAlign->AddS2(s2[curCol - 1]);
			prevDirection = Direction::left;
		}
		curRow = fullCell.row;
		curCol = fullCell.col;
		curAlign->totalLength++;
	} while (fullCell.max == 0);

	return curAlign;
}