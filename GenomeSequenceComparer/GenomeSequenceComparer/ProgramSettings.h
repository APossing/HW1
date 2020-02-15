#pragma once
#include <string>
#include <iostream>
#include <algorithm>
#include <omp.h>
#include <cstring>
using namespace  std;
struct ProgramSettings
{
	static ProgramSettings* GetProgramSettings(int argc, char* argv[])
	{
		ProgramSettings* settings = new ProgramSettings;
		if (argc < 3)
		{
			cout << "Please add parameters... <input file containing both s1 and s2> <0: global, 1: local> <optional: -path path to parameters config file, -parallel (default no), -threadCount 1->maxCores (leave at least 1 core for os, preferably 2, default cores-2)> " << endl;
			return nullptr;
		}

		settings->inFileName = argv[1];
		settings->parametersFileName = "parameters.config";

		settings->isGlobal = true;
		if (argv[2][0] == '1')
			settings->isGlobal = false;
		settings->isParallel = false;
		settings->threadCount = max(omp_get_num_procs() - 2, 1);

		for (int i = 3; i < argc; i++)
		{
			if (argv[i][0] != '-')
			{
				cout << "please use flags for optional parameters (" << argv[i] << ")" << endl;
			}
			if (strcmp(argv[i], "-parallel") == 0)
			{
				settings->isParallel = true;
			}
			else if (strcmp(argv[i], "-path") == 0)
			{
				i++;
				if (i > argc)
				{
					cout << "where is the param value??" << endl;
					return nullptr;
				}
				settings->parametersFileName = argv[i];
			}
			else if (strcmp(argv[i], "-threadCount") == 0)
			{
				i++;
				if (i > argc)
				{
					cout << "where is the param value??" << endl;
					return nullptr;
				}
				settings->threadCount = stoi(argv[i]);
				settings->isParallel = true;
			}
			else
			{
				cout << "Did not understand (" << argv[i] << ")" << endl;;
			}
		}
		return settings;
	}
public:
	bool isGlobal;
	string inFileName;
	string parametersFileName;
	bool isParallel;
	int threadCount;
};

