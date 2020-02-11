#include "DPTable.h"


DPTable::DPTable(int xLength, int yLength)
{
	numRows = xLength;
	numCols = yLength;
	table = new DP_cell * [numRows];
	for (int i = 0; i < numRows; i++)
		table[i] = new DP_cell[numCols];
}

DPTable::~DPTable()
{
	for (int i = 0; i < numRows; i++)
		delete[] table[i];
	delete[] table;
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
			cout << GetCellMax(i, j) << '\t';
		}
		cout << endl;
	}

}

list<DP_cellFull*> DPTable::GetMaxCells()
{
	list<DP_cellFull*> returnT = list<DP_cellFull*>();
	int max = 0; //0,0 will always be 0 so this is ok...
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
		{
			if (GetCellMax(&table[i][j]) < max)
			{

			}
			else if (GetCellMax(&table[i][j]) > max)
			{
				max = GetCellMax(&table[i][j]);
				returnT = list<DP_cellFull*>{ new DP_cellFull(&table[i][j], i, j, max) };
			}
			else
			{
				returnT.push_back({ new DP_cellFull(&table[i][j], i, j, max) });
			}

		}
	}
	return returnT;
}
