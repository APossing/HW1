#pragma once
struct DP_cell {
    short score;
    // add any other field(s) that you may need for the implementation
};


class DPTable
{
public:
    DPTable(int xLength, int yLength);
    DP_cell* FillInCell(int row, int col, int score);
    DP_cell* GetCell(int row, int col);
    bool IsValidCell(int row, int col);
    void PrintTable();
private:
    DP_cell** table;
    int numRows;
    int numCols;

};

