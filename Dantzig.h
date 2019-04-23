#pragma once

#include <vector>

struct DantzigIn
{
	DantzigIn() : inf(100000) {}

	int wierzcholekStartowy;
	int liczbaWierzcholkow;

	const int inf; // brak krawêdzi
	typedef std::vector<std::vector<int>> MacierzGrafu;
	MacierzGrafu macierzGrafu;
};

struct DantzigOut
{
	int32_t dlugoscDrogi;

	typedef std::vector<std::vector<int>> Wynik;
	Wynik maicerzWyniku;

	bool poprawnyWynik;
	std::string opisProblemu;
};

DantzigOut dantzig(DantzigIn inData);