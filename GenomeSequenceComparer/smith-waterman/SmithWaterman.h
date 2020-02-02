#pragma once
#include "pch.h"

#ifdef SMITCHWATERMAN_EXPORTS
#define SMITHWATERMAN_API __declspec(dllexport)
#else
#define SMITHWATERMAN_API __declspec(dllimport)
#endif

extern class SmithWaterman
{
public:
	SmithWaterman(string s1, string s2);
	list<string> Compute();

private:


};

