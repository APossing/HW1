#pragma once
struct DP_cell {
	short substitutionScore;
	short deletionScore;
	short insertionScore;
	// add any other field(s) that you may need for the implementation
};

struct DP_cellFull {
	DP_cell* cell;
	int row;
	int col;
	int max;
	
	DP_cellFull(DP_cell* cell, int row, int col, int max)
	{
		this->max = max;
		this->col = col;
		this->row = row;
		this->cell = cell;
	}
};
#include <iostream>
#include <list>
using namespace std;

class DPTable
{
public:
	DPTable(int xLength, int yLength);
	DP_cell* FillInCell(int row, int col, int score, int delScore, int insertScore);
	DP_cell* GetCell(int row, int col);
	int GetCellMax(int row, int col);
	int GetCellMax(DP_cell* cell);
	bool IsValidCell(int row, int col);
	void PrintTable();
	pair<int, list<pair<int, int>>> GetMaxCells();
private:
	DP_cell** table;
	int numRows;
	int numCols;

};

