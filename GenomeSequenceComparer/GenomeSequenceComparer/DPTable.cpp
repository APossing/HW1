#include "DPTable.h"


DPTable::DPTable(int xLength, int yLength)
{
	numRows = xLength;
	numCols = yLength;
	table = new DP_cell * [numRows];
	for (int i = 0; i < numRows; i++)
		table[i] = new DP_cell[numCols];
}

DP_cell* DPTable::FillInCell(int row, int col, int score)
{
	if (!IsValidCell(row, col))
		return nullptr;
	table[row][col] = DP_cell();
	table[row][col].score = score;
	return &table[row][col];
}

DP_cell* DPTable::GetCell(int row, int col)
{
	return &table[row][col];
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
			cout << table[i][j].score << '\t';
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
			if (table[i][j].score < returnT.first)
			{

			}
			else if (table[i][j].score > returnT.first)
			{
				returnT.first = table[i][j].score;
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
