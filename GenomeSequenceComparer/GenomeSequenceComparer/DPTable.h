#pragma once
using namespace std;
#include <iostream>
#include <list>

struct DP_cell {
	int substitutionScore;
	int deletionScore;
	int insertionScore;
	DP_cell()
	{
		substitutionScore=0;
		deletionScore=0;
		insertionScore=0;
	}
	DP_cell(int sub, int del, int ins)
	{
		substitutionScore = sub;
		deletionScore = del;
		insertionScore = ins;
	}
	DP_cell* DeepCopy()
	{
		return new DP_cell(substitutionScore, deletionScore, insertionScore);
	}
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
	DP_cellFull* DeepCopy()
	{
		return new DP_cellFull(this->cell, this->row, this->col, this->max);
	}
};

class DPTable
{
public:
	DPTable(int xLength, int yLength);
	~DPTable();
	DP_cell* FillInCell(int row, int col, int score, int delScore, int insertScore);
	DP_cell* GetCell(int row, int col);
	int GetCellMax(int row, int col);
	int GetCellMax(DP_cell* cell);
	bool IsValidCell(int row, int col);
	void PrintTable();
	list<DP_cellFull*> GetMaxCells();
private:
	DP_cell** table;
	int numRows;
	int numCols;

};

