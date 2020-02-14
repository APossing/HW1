#include "OptimalAlignmentLib.h"
#include <vector>
#include <omp.h>

OptimalAlignment::OptimalAlignment(string s1, string s2, int match, int misMatch, int h, int g)
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
	delete this->table;
	this->table = new DPTable(s1.length() + 1, s2.length() + 1);
}


bool OptimalAlignment::RunNeedlemanWunsch()
{
	init();

	for (int row = 0; row < s1.length() + 1; row++)
		for (int col = 0; col < s2.length() + 1; col++)
			CalculateCell(row, col);

	//	table->PrintTable();
	return true;
}

bool OptimalAlignment::RunSmithWaterman()
{
	init();

	for (int row = 0; row < s1.length() + 1; row++)
		for (int col = 0; col < s2.length() + 1; col++)
			CalculateCell(row, col, 0);

	//	table->PrintTable();
	return true;
}

Alignment* OptimalAlignment::GetGlobalMaxStrings()
{
	return TraceBackGlobal(s1.length(), s2.length(), new Alignment());
}
list<Alignment*> OptimalAlignment::GetLocalMaxStrings()
{
	list<DP_cellFull*> maxCells = table->GetMaxCells();
	list<Alignment*> returnAlignments = list<Alignment*>();
	for (auto maxCell : maxCells)
		returnAlignments.push_back(TraceBackLocal(maxCell->row, maxCell->col, new Alignment));
	return returnAlignments;
}

DP_cell* OptimalAlignment::CalculateCell(int row, int col)
{
	if (row == 0 && col == 0)
		return table->FillInCell(row, col, 0, 0, 0);
	if (row == 0)
		return table->FillInCell(row, col, -1 * col, -1 * col, -1 * col);
	if (col == 0)
		return table->FillInCell(row, col, -1 * row, -1 * row, -1 * row);

	int subScore = GetMaxSubScore(row, col);
	int delScore = GetMaxDeletionScore(row, col);
	int insScore = GetMaxInsertionScore(row, col);

	return table->FillInCell(row, col, subScore, delScore, insScore);
}

DP_cell* OptimalAlignment::CalculateCell(int row, int col, int min)
{
	DP_cell* cell = CalculateCell(row, col);
	if (cell->deletionScore < min)
		cell->deletionScore = min;

	if (cell->insertionScore < min)
		cell->insertionScore = min;

	if (cell->substitutionScore < min)
		cell->substitutionScore = min;

	return cell;
}

int OptimalAlignment::GetMaxSubScore(int row, int col)
{
	int matchScore = misMatch;
	if (s1[row - 1] == s2[col - 1])
		matchScore = match;

	auto diagCell = GetCalculatedCell(row - 1, col - 1);

	return DPTable::GetCellMax(diagCell) + matchScore;
}
int OptimalAlignment::GetMaxDeletionScore(int row, int col)
{
	DP_cell* upCell = GetCalculatedCell(row - 1, col);
	int max = upCell->deletionScore + g;

	if (upCell->insertionScore + h + g > max)
		max = upCell->insertionScore + h + g;
	if (upCell->substitutionScore + h + g > max)
		max = upCell->substitutionScore + h + g;

	return max;
}

int OptimalAlignment::GetMaxInsertionScore(int row, int col)
{
	DP_cell* leftCell = GetCalculatedCell(row, col - 1);
	int max = leftCell->deletionScore + h + g;

	if (leftCell->insertionScore + g > max)
		max = leftCell->insertionScore + g;
	if (leftCell->substitutionScore + h + g > max)
		max = leftCell->substitutionScore + h + g;

	return max;
}

DP_cell* OptimalAlignment::GetCalculatedCell(int row, int col)
{
	if (table->IsValidCell(row, col))
	{
		DP_cell* cell = table->GetCell(row, col);
		if (cell == nullptr)
			cell = CalculateCell(row, col);
		return cell;
	}
	return nullptr;
}

list<DP_cellFull> OptimalAlignment::GetMaxAdjacentCells(int row, int col, Direction prevDirection) const
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
	const int max = table->GetCellMax(cell);

	if (row == 0)
	{
		maxAdjacentSquares.emplace_back(table->GetCell(row, col - 1), row, col - 1, max);
		return maxAdjacentSquares;
	}
	if (col == 0)
	{
		maxAdjacentSquares.emplace_back(table->GetCell(row - 1, col), row - 1, col, max);
		return maxAdjacentSquares;
	}
	if (cell->deletionScore == max)
		maxAdjacentSquares.emplace_back(table->GetCell(row - 1, col), row - 1, col, max);
	if (cell->insertionScore == max)
		maxAdjacentSquares.emplace_back(table->GetCell(row, col - 1), row, col - 1, max);
	if (cell->substitutionScore == max)
		maxAdjacentSquares.emplace_back(table->GetCell(row - 1, col - 1), row - 1, col - 1, max);


	return maxAdjacentSquares;
}

void OptimalAlignment::AddPointsToAlignment(int curRow, int curCol, Alignment* alignment)
{
	if (alignment->s2.length() > 0 && alignment->s2[0] == '-')
	{
		if (alignment->s2.length() < 2 || alignment->s2[1] != '-')
			alignment->openingGaps++;
		alignment->gaps++;
	}

	else if (alignment->s1.length() > 0 && alignment->s1[0] == '-')
	{
		if (alignment->s1.length() < 2 || alignment->s1[1] != '-')
			alignment->openingGaps++;
		alignment->gaps++;
	}
	else
	{
		if (s1[curRow] == s2[curCol])
			alignment->matches++;
		else
			alignment->mismatches++;
	}
	alignment->totalLength++;
}


Alignment* OptimalAlignment::TraceBackGlobal(int row, int col, Alignment* alignment)
{
	Direction prevDirection = Direction::diag;
	do
	{
		const auto maxCell = GetMaxAdjacentCells(row, col, prevDirection).front();

		if (maxCell.row < row && maxCell.col < col)
		{
			//substitution
			alignment->AddS1(s1[row - 1]);
			alignment->AddS2(s2[col - 1]);
			prevDirection = Direction::diag;
		}
		else if (maxCell.row < row)
		{
			//deletion
			alignment->AddS1(s1[row - 1]);
			alignment->AddS2('-');
			prevDirection = Direction::up;
		}
		else
		{
			//insertion
			alignment->AddS1('-');
			alignment->AddS2(s2[col - 1]);
			prevDirection = Direction::left;
		}
		row = maxCell.row;
		col = maxCell.col;
		AddPointsToAlignment(row, col, alignment);
	} while (row != 0 || col != 0);

	return alignment;
}

Alignment* OptimalAlignment::TraceBackLocal(int row, int col, Alignment* alignment)
{
	DP_cellFull maxCell(nullptr, 0, 0, 0);
	Direction prevDirection = Direction::diag;

	do
	{
		maxCell = GetMaxAdjacentCells(row, col, prevDirection).front();
		if (maxCell.row < row && maxCell.col < col)
		{
			//substitution
			alignment->AddS1(s1[row - 1]);
			alignment->AddS2(s2[col - 1]);
			prevDirection = Direction::diag;

		}
		else if (maxCell.row < row)
		{
			//deletion
			alignment->AddS1(s1[row - 1]);
			alignment->AddS2('-');
			prevDirection = Direction::up;
		}
		else
		{
			//insertion
			alignment->AddS1('-');
			alignment->AddS2(s2[col - 1]);
			prevDirection = Direction::left;
		}
		row = maxCell.row;
		col = maxCell.col;
		AddPointsToAlignment(row, col, alignment);
	} while (maxCell.max != 0);

	return alignment;
}