#include "DPTable.h"


DPTable::DPTable(int xLength, int yLength)
{
	numRows = xLength;
	numCols = yLength;
	table = new DP_cell * [numRows];
	for (int i = 0; i < numRows; i++)
		table[i] = new DP_cell[numCols];
}

DP_cell* DPTable::FillInCell(int row, int col, int subScore, int delScore, int insScore)
{
	if (!IsValidCell(row, col))
		return nullptr;
	table[row][col] = DP_cell();
	table[row][col].substitutionScore = subScore;
	table[row][col].deletionScore = delScore;
	table[row][col].insertionScore = insScore;
	return &table[row][col];
}

DP_cell* DPTable::GetCell(int row, int col)
{
	return &table[row][col];
}

int DPTable::GetCellMax(int row, int col)
{
	return GetCellMax(&table[row][col]);
}

int DPTable::GetCellMax(DP_cell* cell)
{
	int max = cell->substitutionScore;
	if (cell->deletionScore > max)
		max = cell->deletionScore;
	if (cell->insertionScore > max)
		max = cell->insertionScore;
	return max;
}

bool DPTable::IsValidCell(int row, int col)
{
	if (row >= 0 && col >= 0 && row < numRows && col < numCols)
		return true;
	return false;
}

void DPTable::PrintTable()
{
	cout << endl;
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
		{
			cout << table[i][j].substitutionScore << '\t';
		}
		cout << endl;
	}

}

pair<int, list<pair<int, int>>> DPTable::GetMaxCells()
{
	pair<int, list<pair<int, int>>> returnT = pair<int, list<pair<int, int>>>();
	returnT.first = 0;
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
		{
			if (table[i][j].substitutionScore < returnT.first)
			{

			}
			else if (table[i][j].substitutionScore > returnT.first)
			{
				returnT.first = table[i][j].substitutionScore;
				returnT.second = list<pair<int, int>>{ {i,j} };
			}
			else
			{
				returnT.second.push_back({ i,j });
			}

		}
	}
	return returnT;
}
