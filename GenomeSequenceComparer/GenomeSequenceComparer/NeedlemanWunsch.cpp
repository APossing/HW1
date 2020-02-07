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
	this->TracebackPrevDirection = none;
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

DP_cell* NeedlemanWunsch::GetRealCellScores(int row, int col, Direction incomingDirection)
{
	if (row < 0 || col < 0)
		return nullptr;

	DP_cell* trueCell = table->GetCell(row, col);
	DP_cell* recalcCell;
	if (incomingDirection == Direction::left)
	{
		recalcCell = new DP_cell();
		recalcCell->deletionScore = trueCell->deletionScore;
		recalcCell->insertionScore = trueCell->insertionScore + h;
		recalcCell->substitutionScore = trueCell->substitutionScore + h;
	}
	else if (incomingDirection == Direction::up)
	{
		recalcCell = new DP_cell();
		recalcCell->deletionScore = trueCell->deletionScore + h;
		recalcCell->insertionScore = trueCell->insertionScore;
		recalcCell->substitutionScore = trueCell->substitutionScore + h;
	}
	else
	{
		recalcCell = table->GetCell(row, col);
	}

	return recalcCell;
}

list<pair<string, string>> NeedlemanWunsch::TraceBack(int row, int col, string cur1, string cur2)
{
	if (row == 0 && col == 0)
		return { {cur1,cur2} };

	list<DP_cellFull> maxAdjacentSquares = FindMaxAdjacentSquares(row, col);
	if (maxAdjacentSquares.empty())
	{
		return { {"why did this break", "why did this berak"} };
	}

	list<pair<string, string>> returnList = list<pair<string, string>>();

	//max is 0 and one is diagonal
	if (maxAdjacentSquares.front().max == 0 && table->GetCellMax(GetRealCellScores(row - 1, col - 1, diagonal)) == maxAdjacentSquares.front().max)
	{
		returnList.merge(TraceBack(row - 1, col - 1, s1[row - 1] + cur1, s2[col - 1] + cur2));
	}
	else
	{
		for (auto fullCell : maxAdjacentSquares)
		{
			if (fullCell.row < row && fullCell.col < col)
				returnList.merge(TraceBack(fullCell.row, fullCell.col, s1[row - 1] + cur1, s2[col - 1] + cur2));
			else if (fullCell.row < row)
				returnList.merge(TraceBack(fullCell.row, fullCell.col, s1[row - 1] + cur1, "-" + cur2));
			else
				returnList.merge(TraceBack(fullCell.row, fullCell.col, "-" + cur1, s2[col - 1] + cur2));

		}
	}

	return returnList;
}

list<DP_cellFull> NeedlemanWunsch::FindMaxAdjacentSquares(int row, int col)
{
	int max = 0;
	bool isMax = false;

	list<DP_cellFull> returnList = list<DP_cellFull>();

	DP_cell* fixedScoreCell = GetRealCellScores(row - 1, col, Direction::up);
	if (fixedScoreCell != nullptr)
	{
		max = table->GetCellMax(fixedScoreCell);
		isMax = true;

		DP_cellFull fullCell = DP_cellFull();
		fullCell.cell = fixedScoreCell;
		fullCell.col = col;
		fullCell.max = max;
		fullCell.row = row - 1;
		returnList.push_back(fullCell);
	}

	fixedScoreCell = GetRealCellScores(row, col - 1, Direction::left);
	if (fixedScoreCell != nullptr)
	{
		int tempMaxVal = table->GetCellMax(fixedScoreCell);
		if (tempMaxVal > max || !isMax)
		{
			max = tempMaxVal;
			returnList = list<DP_cellFull>();
		}
		if (tempMaxVal == max)
		{
			DP_cellFull fullCell = DP_cellFull();
			fullCell.cell = fixedScoreCell;
			fullCell.col = col - 1;
			fullCell.max = max;
			fullCell.row = row;
			returnList.push_back(fullCell);
		}
	}

	fixedScoreCell = GetRealCellScores(row - 1, col - 1, Direction::diagonal);
	if (fixedScoreCell != nullptr)
	{
		int tempMaxVal = table->GetCellMax(fixedScoreCell);
		if (tempMaxVal > max || !isMax)
		{
			max = tempMaxVal;
			returnList = list<DP_cellFull>();
		}
		if (tempMaxVal == max)
		{
			DP_cellFull fullCell = DP_cellFull();
			fullCell.cell = fixedScoreCell;
			fullCell.col = col - 1;
			fullCell.max = max;
			fullCell.row = row - 1;
			returnList.push_back(fullCell);
		}
	}

	return returnList;
}
