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

DP_cell* DPTable::FillInCell(int row, int col, int subScore, int delScore, int insScore) const
{
	if (!IsValidCell(row, col))
		return nullptr;
	table[row][col] = DP_cell();
	table[row][col].substitutionScore = subScore;
	table[row][col].deletionScore = delScore;
	table[row][col].insertionScore = insScore;
	return &table[row][col];
}

DP_cell* DPTable::GetCell(int row, int col) const
{
	return &table[row][col];
}

int DPTable::GetCellMax(int row, int col) const
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

bool DPTable::IsValidCell(int row, int col) const
{
	if (row >= 0 && col >= 0 && row < numRows && col < numCols)
		return true;
	return false;
}

void DPTable::PrintTable() const
{
	cout << endl;
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
		{
			auto cell = table[i][j];
			cout << "(" << cell.insertionScore << ")-(" << cell.substitutionScore << ")-(" << cell.deletionScore << ")" << '\t';
		}
		cout << endl;
	}

}

list<DP_cellFull*> DPTable::GetMaxCells() const
{
	list<DP_cellFull*> returnT = list<DP_cellFull*>();
	int max = 0; //0,0 will always be 0 so this is ok...
	for (int row = 0; row < numRows; row++)
	{
		for (int col = 0; col < numCols; col++)
		{
			if (GetCellMax(&table[row][col]) < max)
			{

			}
			else if (GetCellMax(&table[row][col]) > max)
			{
				max = GetCellMax(&table[row][col]);
				returnT = list<DP_cellFull*>{ new DP_cellFull(&table[row][col], row, col, max) };
			}
			else
			{
				returnT.push_back({ new DP_cellFull(&table[row][col], row, col, max) });
			}
		}
	}
	return returnT;
}
