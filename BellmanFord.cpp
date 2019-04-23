#include <iostream>
#include "BellmanFord.h"

using namespace std;

const int MAXINT = 2147483647;    // Najwiêksza liczba ca³kowita

								  // Typy danych
struct slistEl
{
	slistEl * next;
	int v, w;
};

// Zmienne globalne
int m, n;                          // Liczba krawêdzi i wierzcho³ków w grafie
slistEl ** A;                     // Tablica dynamiczna list s¹siedztwa
long long * d;                    // Tablica kosztów dojœcia
int * p;                          // Tablica poprzedników

								  // Funkcja wyznacza najkrótsze œcie¿ki
								  // v - wierzcho³ek startowy
								  // Wyjœcie:
								  // true  - wyniki w d i p
								  // false - graf zawiera ujemny cykl
								  //------------------------------------
bool BF(int v)
{
	int i, x;
	bool test;
	slistEl * pv;

	d[v] = 0;                       // Zerujemy koszt dojœcia do v
	for (i = 1; i < n; i++)          // Pêtla relaksacji
	{
		test = true;                  // Oznacza, ¿e algorytm nie wprowadzi³ zmian do d i p
		for (x = 0; x < n; x++)        // Przechodzimy przez kolejne wierzcho³ki grafu
			for (pv = A[x]; pv; pv = pv->next) // Przegl¹damy listê s¹siadów wierzcho³ka x
				if (d[pv->v] > d[x] + pv->w) // Sprawdzamy warunek relaksacji
				{
					test = false;           // Jest zmiana w d i p
					d[pv->v] = d[x] + pv->w; // Relaksujemy krawêdŸ z x do jego s¹siada
					p[pv->v] = x;           // Poprzednikiem s¹siada bêdzie x
				}
		if (test) return true;         // Jeœli nie by³o zmian, to koñczymy
	}

	// Sprawdzamy istnienie ujemnego cyklu

	for (x = 0; x < n; x++)
		for (pv = A[x]; pv; pv = pv->next)
			if (d[pv->v] > d[x] + pv->w) return false; // ujemny cykl!!

	return true;
}

BellmanFordOut bellmanFord(BellmanFordIn inData)
{
	BellmanFordOut outData;
	outData.poprawnyWynik = false;

	int i, v, sptr, *S;
	uint32_t x;
	slistEl *rv, *pv;

	v = inData.wierzcholekStartowy;
	n = inData.liczbaWierzcholkow;

	A = new slistEl *[n];          // Tworzymy tablicê list s¹siedztwa
	d = new long long[n];          // Tworzymy tablicê kosztów dojœcia
	p = new int[n];                // Tworzymy tablice poprzedników
	for (i = 0; i < n; i++)          // Inicjujemy struktury danych
	{
		d[i] = MAXINT;
		p[i] = -1;
		A[i] = NULL;
	}

	for (x = 0; x < inData.listaSasiadow.size(); x++)
	{
		for (const auto &krawedz : inData.listaSasiadow[x])
		{
			pv = new slistEl;             // Tworzymy element listy
			pv->v = krawedz.indeksCelu;   // Inicjujemy go
			pv->w = krawedz.waga;
			pv->next = A[x];              // Dodajemy go na pocz¹tek listy s¹siadów wierzcho³ka x
			A[x] = pv;
		}
	}

	// Wyznaczamy najkrótsze œcie¿ki algorytmem Bellmana-Forda
	if (BF(v))
	{
		outData.poprawnyWynik = true;

		S = new int[n];              // Tworzymy prosty stos
		sptr = 0;

		i = inData.liczbaWierzcholkow - 1;

		for (x = i; x != -1; x = p[x]) // Wierzcho³ki œcie¿ki umieszczamy na stosie
			S[sptr++] = x;            // w kolejnoœci od ostatniego do pierwszego

		outData.najkrotszaDroga = d[i];

		while (sptr)                 // Wierzcho³ki ze stosu zapisujemy
			outData.droga.push_back(S[--sptr]); // w kolejnoœci od pierwszego do ostatniego

		for (i = 0; i < n; i++)
		{
			outData.D.push_back(d[i]);
			outData.p.push_back(p[i]);
		}

		delete[] S;                  // Usuwamy stos
	}
	else outData.opisProblemu = "Negative Cycle found!";

	// Usuwamy struktury dynamiczne
	for (i = 0; i < n; i++)
	{
		pv = A[i];
		while (pv)
		{
			rv = pv;
			pv = pv->next;
			delete rv;
		}
	}

	delete[] A;
	delete[] d;
	delete[] p;

	return outData;
}
