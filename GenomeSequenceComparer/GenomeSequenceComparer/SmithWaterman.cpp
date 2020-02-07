#include "SmithWaterman.h"

SmithWaterman::SmithWaterman(string s1, string s2, double match, double misMatch, double h, double g)
{
	this->s1 = s1;
	this->s2 = s2;
	this->match = match;
	this->misMatch = misMatch;
	this->h = h;
	this->g = g;
	this->table = new DPTable(s1.length() + 1, s2.length() + 1);
}
//
//DP_cell* SmithWaterman::GetCalculatedCell(int row, int col)
//{
//	if (table->IsValidCell(row, col))
//	{
//		DP_cell* c = table->GetCell(row, col);
//		if (c == nullptr)
//			c = CalculateCell(row, col);
//		return c;
//	}
//	return nullptr;
//}
//
//
//
//int SmithWaterman::GetMaxScore(int row, int col, int matchScore)
//{
//	////deletion
//	//DP_cell* c = GetCalculatedCell(row - 1, col);
//	//int max = c->score + this->g;
//
//	////insertion
//	//c = GetCalculatedCell(row, col - 1);
//	//if (max < c->score + g)
//	//	max = c->score + g;
//
//	////diag
//	//c = GetCalculatedCell(row - 1, col - 1);
//	//if (max < c->score + matchScore)
//	//	max = c->score + matchScore;
//	////0
//	//if (max < 0)
//	//	max = 0;
//
//	//return max;
//	return 1;
//
//}
//
//
//DP_cell* SmithWaterman::CalculateCell(int row, int col)
//{
//	if (row == 0 || col == 0)
//		return table->FillInCell(row, col, 0,0,0);
//
//	int matchScore = misMatch;
//	if (s1[row - 1] == s2[col - 1])
//		matchScore = match;
//
//	int maxValue = GetMaxScore(row, col, matchScore);
//	//fix me!
//	return table->FillInCell(row, col, maxValue, -1,1);
//
//
//}
//
//int SmithWaterman::FindMaxAdjacentSquareValue(int row, int col)
//{
//	//int max = table->GetCell(row - 1, col)->score;
//	//int tempVal = table->GetCell(row, col - 1)->score;
//	//if (tempVal > max)
//	//	max = tempVal;
//	//tempVal = table->GetCell(row - 1, col - 1)->score;
//	//if (tempVal > max)
//	//	max = tempVal;
//
//	//return max;
//	return 1;
//}
//
//list<pair<string, string>> SmithWaterman::TraceBack(int row, int col, string cur1, string cur2)
//{
//	//if ((table->GetCell(row, col)->score == 0))
//	//	return { {cur1,cur2} };
//
//	//int max = FindMaxAdjacentSquareValue(row, col);
//	//list<pair<string, string>> returnList = list<pair<string, string>>();
//
//	////if diag = 0
//	//if (table->GetCell(row - 1, col - 1)->score == 0)
//	//{
//	//	return { { s1[row - 1] + cur1, s2[col - 1] + cur2 } };
//	//}
//	//else
//	//{
//	//	//insertion (up)
//	//	if (table->GetCell(row - 1, col)->score == max)
//	//	{
//	//		returnList.merge(TraceBack(row - 1, col, s1[row - 1] + cur1, "-" + cur2));
//	//	}
//
//	//	//deletion (left)
//	//	if (table->GetCell(row, col - 1)->score == max)
//	//	{
//	//		returnList.merge(TraceBack(row, col - 1, "-" + cur1, s2[col - 1] + cur2));
//	//	}
//
//	//	//direct (diag)
//	//	if (table->GetCell(row - 1, col - 1)->score == max)
//	//	{
//	//		returnList.merge(TraceBack(row - 1, col - 1, s1[row - 1] + cur1, s2[col - 1] + cur2));
//	//	}
//	//}
//
//	//return returnList;
//	return {};
//
//}
//
//list<pair<string, string>> SmithWaterman::GetMaxStrings()
//{
//	auto t = table->GetMaxCells();
//	list<pair<string, string>> returnT = list<pair<string,string>>();
//	for (auto it : t.second)
//	{
//		returnT.merge(TraceBack(it.first, it.second, "", ""));
//	}
//	return returnT;
//}
//
//bool SmithWaterman::Run()
//{
//	for (int i = 0; i < s1.length() + 1; i++)
//	{
//		for (int j = 0; j < s2.length() + 1; j++)
//		{
//			CalculateCell(i, j);
//		}
//	}
//
//	table->PrintTable();
//	return true;
//}


