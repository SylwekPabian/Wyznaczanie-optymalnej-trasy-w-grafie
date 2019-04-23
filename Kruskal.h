#pragma once

#include <string>
#include <vector>

struct KruskalIn
{
	KruskalIn() : inf(100000) {}

	int wierzcholekStartowy;
	int liczbaWierzcholkow;

	const int inf; // brak krawedzi
	typedef std::vector<std::vector<int>> MacierzGrafu;
	MacierzGrafu macierzGrafu;
};

struct KruskalOut
{
	int32_t dlugoscDrogi;

	typedef std::vector<std::vector<int>> Wynik;
	Wynik maicerzWyniku;

	bool poprawnyWynik;
	std::string opisProblemu;
};

KruskalOut kruskalMinimalne(KruskalIn inData);
KruskalOut kruskalMaksymalne(KruskalIn inData);