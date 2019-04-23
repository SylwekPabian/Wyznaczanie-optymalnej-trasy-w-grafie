#pragma once

#include <vector>

struct BellmanFordIn
{
	int wierzcholekStartowy;
	int liczbaWierzcholkow;

	struct Krawedz {
		int indeksCelu;
		int waga;
		Krawedz(int i, int w) : indeksCelu(i), waga(w) {}
	};

	std::vector<std::vector<Krawedz>> listaSasiadow;
};

struct BellmanFordOut
{
	std::vector<int> D;
	std::vector<int> p;
	std::vector<uint32_t> droga; // kolejne wierzcholki
	int32_t najkrotszaDroga;
	bool poprawnyWynik;
	std::string opisProblemu;
};

BellmanFordOut bellmanFord(BellmanFordIn inData);