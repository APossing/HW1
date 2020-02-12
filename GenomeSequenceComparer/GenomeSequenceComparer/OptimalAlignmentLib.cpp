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

list<Alignment*> OptimalAlignment::GetGlobalMaxStrings()
{
	return TraceBackGlobal(s1.length(), s2.length(), new Alignment());
}
list<list<Alignment*>> OptimalAlignment::GetLocalMaxStrings()
{
	list<DP_cellFull*> startingLocals = table->GetMaxCells();
	list<list<Alignment*>> returnAlignments = list<list<Alignment*>>();
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

list<DP_cellFull> OptimalAlignment::GetMaxAdjacentCells(int row, int col)
{
	list<DP_cellFull> maxAdjacentSquares = list<DP_cellFull>();
	DP_cell* cell = table->GetCell(row, col);
	int max = table->GetCellMax(row, col);

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

list<Alignment*> OptimalAlignment::TraceBackGlobal(int row2, int col2, Alignment* alignment2)
{
	//list<Alignment*>** memoize = new list<Alignment*> * [s1.length() + 1];
	//for (int i = 0; i < s1.length() + 1; i++)
	//	memoize[i] = new list<Alignment*>[s2.length()];
	//
	int curRow = row2;
	int curCol = col2;
	int run = 0;
	Alignment* curAlign = alignment2;

	list<Alignment*> completeAligns = list<Alignment*>();
	Alignment* curOptimalAlign = nullptr;
	vector<pair<DP_cellFull*, Alignment*>> ToDo = vector<pair<DP_cellFull*, Alignment*>>();

	do
	{
		//start first
		if (table->GetCellMax(curRow, curCol) > curAlign->optimalScore)
			curAlign->optimalScore = table->GetCellMax(curRow, curCol);

		list<DP_cellFull> maxAdjacentSquares = GetMaxAdjacentCells(curRow, curCol);

		for (auto fullCell : maxAdjacentSquares)
		{
			if (fullCell.row != maxAdjacentSquares.back().row || fullCell.col != maxAdjacentSquares.back().col)
			{
				ToDo.push_back({ fullCell.DeepCopy(), curAlign->DeepCopy() });
			}
			else
			{
				if (fullCell.row < curRow && fullCell.col < curCol)
				{
					if (s1[curRow - 1] == s2[curCol - 1])
						curAlign->matches++;
					else
						curAlign->mismatches++;

					curAlign->AddS1(s1[curRow - 1]);
					curAlign->AddS2(s2[curCol - 1]);
				}
				else if (fullCell.row < curRow)
				{
					if (curAlign->s2.length() > 0 && curAlign->s2[0] != '-')
						curAlign->openingGaps++;
					curAlign->gaps++;
					curAlign->AddS1(s1[curRow - 1]);
					curAlign->AddS2('-');
				}
				else
				{
					if (curAlign->s2.length() > 0 && curAlign->s2[0] != '-')
						curAlign->openingGaps++;
					curAlign->gaps++;
					curAlign->AddS1('-');
					curAlign->AddS2(s2[curCol - 1]);
				}
				curRow = fullCell.row;
				curCol = fullCell.col;
			}
		}
	} while (curRow != 0 && curCol != 0);
	curOptimalAlign = curAlign;


	omp_set_dynamic(0);     // disable dynamic teams
	omp_set_num_threads(16); // Use p threads for all consecutive parallel regions
	//cout << omp_get_num_threads() << endl;
	//cout << omp_get_num_procs() << endl;
	#pragma omp parallel for schedule(static) shared(ToDo)
	for (int i = ToDo.size() - 1; i >= 0; i--)
	{
		cout << omp_get_thread_num() << endl;
		bool complete = false;
		Alignment* myCurAlignment = ToDo[i].second;
		int myCurRow = ToDo[i].first->row;
		int myCurCol = ToDo[i].first->col;

		list<pair<DP_cellFull*, Alignment*>> myToDo = list<pair<DP_cellFull*, Alignment*>>();

		do
		{
			if (table->GetCellMax(myCurRow, myCurCol) > myCurAlignment->optimalScore)
				myCurAlignment->optimalScore = table->GetCellMax(myCurRow, myCurCol);

			list<DP_cellFull> maxAdjacentSquares = GetMaxAdjacentCells(myCurRow, myCurCol);

			for (auto fullCell : maxAdjacentSquares)
			{
				if (fullCell.row != maxAdjacentSquares.back().row || fullCell.col != maxAdjacentSquares.back().col)
				{
					myToDo.push_front({ fullCell.DeepCopy(), myCurAlignment->DeepCopy() });
				}
				else
				{
					if (fullCell.row < myCurRow && fullCell.col < myCurCol)
					{
						if (s1[myCurRow - 1] == s2[myCurCol - 1])
							myCurAlignment->matches++;
						else
							myCurAlignment->mismatches++;

						myCurAlignment->AddS1(s1[myCurRow - 1]);
						myCurAlignment->AddS2(s2[myCurCol - 1]);
					}
					else if (fullCell.row < myCurRow)
					{
						if (myCurAlignment->s2.length() > 0 && myCurAlignment->s2[0] != '-')
							myCurAlignment->openingGaps++;
						myCurAlignment->gaps++;
						myCurAlignment->AddS1(s1[myCurRow - 1]);
						myCurAlignment->AddS2('-');
					}
					else
					{
						if (myCurAlignment->s2.length() > 0 && myCurAlignment->s2[0] != '-')
							myCurAlignment->openingGaps++;
						myCurAlignment->gaps++;
						myCurAlignment->AddS1('-');
						myCurAlignment->AddS2(s2[myCurCol - 1]);
					}
					myCurRow = fullCell.row;
					myCurCol = fullCell.col;
				}
			}

			if (myCurRow == 0 && myCurCol == 0)
			{
				#pragma omp critical
				{
					run++;
					if (curOptimalAlign->GetScore(match, misMatch, h, g) < myCurAlignment->GetScore(match, misMatch, h, g))
					{
						delete curOptimalAlign;
						curOptimalAlign = myCurAlignment;
					}
					else
					{
						delete myCurAlignment;
					}
				}


				if (myToDo.empty())
				{
					complete = true;
				}
				else
				{
					myCurAlignment = myToDo.front().second;
					myCurRow = myToDo.front().first->row;
					myCurCol = myToDo.front().first->col;
					myToDo.pop_front();
				}
			}

		} while (!complete);
		cout << i << ":" << omp_get_thread_num() << endl;
	}




	return { curOptimalAlign };
}

list<Alignment*> OptimalAlignment::TraceBackLocal(int row, int col, Alignment* alignment)
{
	if (row == 0 && col == 0)
		return { alignment };
	alignment->totalLength++;
	list<DP_cellFull> maxAdjacentSquares = GetMaxAdjacentCells(row, col);
	if (maxAdjacentSquares.empty())
		return { new Alignment() };

	list<Alignment*> returnList = list<Alignment*>();

	//max is 0 and one is diagonal
	if (table->GetCell(row, col)->substitutionScore == 0 && table->GetCellMax(row, col) == 0)
	{
		alignment->AddS1(s1[row - 1]);
		alignment->AddS2(s2[col - 1]);
		return { alignment };
	}
	for (auto fullCell : maxAdjacentSquares)
	{
		if (fullCell.row < row && fullCell.col < col)
		{
			if (s1[row - 1] == s2[col - 1])
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
		returnList.merge(TraceBackLocal(fullCell.row, fullCell.col, alignment->DeepCopy()));
	}

	return returnList;
}