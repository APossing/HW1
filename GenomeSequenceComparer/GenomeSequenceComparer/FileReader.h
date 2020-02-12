#pragma once
using namespace std;
#include <String>
#include <fstream>

struct Parameters
{
public:
	Parameters(int h, int g, int match, int mismatch)
	{
		this->h = h;
		this->g = g;
		this->match = match;
		this->mismatch = mismatch;
	}

	Parameters()
	{
		this->h = 0;
		this->g = 0;
		this->match = 0;
		this->mismatch = 0;
	}

	int h;
	int g;
	int match;
	int mismatch;
};


struct MultiSequenceFasta
{
public:
	string s1;
	string s2;
	string name1;
	string name2;
};

struct FileReader
{
public:
	static MultiSequenceFasta ReadFile(string fileName)
	{
		MultiSequenceFasta info = MultiSequenceFasta();
		ifstream file = ifstream(fileName);
		string line = "";

		file >> line;
		line.erase(0, 1);
		info.name1 = line;

		getline(file, line);
		getline(file, line);
		while (line.length() > 0 && line[0] != '>')
		{
			info.s1 += line;
			getline(file, line);
		}

		file >> line;
		line.erase(0, 1);
		info.name2 = line;

		getline(file, line);
		while (!file.eof())
		{
			getline(file, line);
			info.s2 += line;
		}


		file.close();
		return info;
	}
	static Parameters ReadParameters(string fileName)
	{
		Parameters params = Parameters();
		ifstream file = ifstream(fileName);
		string line = "";
		file >> line;
		file >> params.match;

		file >> line;
		file >> params.mismatch;

		file >> line;
		file >> params.h;

		file >> line;
		file >> params.g;

		file.close();
		return params;
	}
};

