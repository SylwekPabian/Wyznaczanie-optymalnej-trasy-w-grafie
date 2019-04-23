// ConsoleApplication1.cpp: Okresla punkt wejscia dla aplikacji konsoli.
//

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <list>
#include <stack>

#include "Kruskal.h"

//#define inf 100000

using namespace std;



struct liczba
{
	int zmienna1;
	int zmienna2;
	int wartosc;
	liczba *next;

};

//dodaj element do stosu (root)
void dodaj(liczba **root, int z1, int z2, int wart)
{
	liczba *nowa = new liczba;
	nowa->zmienna1 = z1;
	nowa->zmienna2 = z2;
	nowa->wartosc = wart;
	nowa->next = *root;
	*root = nowa;
}

struct listaPom {            // struktura listy sprawdzajacej
	int wartosc;
	listaPom *head;         // poczatek
	listaPom *next;         // kolejny el struktury
	listaPom();

	void dodaj(int);
	void zamien(int, int);
	int znajdz(int, int);
	int rozmiar();
};

listaPom::listaPom() {
	head = 0;
	next = 0;
};

void listaPom::dodaj(int wart) {
	listaPom *nowa = new listaPom;
	nowa->wartosc = wart;
	if (head == 0) {
		head = nowa;
	}
	else {
		listaPom *temp = head;
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = nowa;
		nowa->next = 0;
	}
};

int listaPom::znajdz(int x, int y) {
	listaPom *temp = head;
	int i = 1;
	int X, Y;
	int doNadpisania;

	if (x == 0) {
		X = temp->wartosc;
	}
	else {
		while (i <= x) {
			temp = temp->next;
			X = temp->wartosc;
			i++;
		}
	}
	i = 1;
	temp = head;

	if (y == 0) {
		Y = temp->wartosc;
	}
	else {
		while (i <= y) {
			temp = temp->next;
			Y = temp->wartosc;
			i++;
		}
	}
	if (X == Y)
		return 0;
	else
		return 1;
};

int listaPom::rozmiar() {
	listaPom *temp = head;
	int i = 0;
	while (temp) {
		temp = temp->next;
		i++;
	}
	return i;
}

void listaPom::zamien(int y, int x) {
	listaPom *temp = head;              // wskaznik na poczatek
	int i = 1;
	int X, Y;	//wartosc w x i y

	if (x == 0) {
		X = temp->wartosc;
	}
	else {
		while (i <= x) {
			temp = temp->next;
			X = temp->wartosc;
			i++;
		}
	}
	i = 1;
	temp = head;

	if (y == 0) {
		Y = temp->wartosc;
	}
	else {
		while (i <= y) {
			temp = temp->next;
			Y = temp->wartosc;
			i++;
		}
	}
	if (Y != X) {
		temp->wartosc = X;	// temp->wartosc aktualnie wskazuje na wartosc y
	}
	temp = head;

	while (temp) {
		if (temp->wartosc == Y) {     // gdy wartosc == tej do zamiany
			temp->wartosc = X;      // zamien ja na ta do zamiany
		}
		temp = temp->next;                  // nastepny element
	}
}

KruskalOut kruskalMinimalne(KruskalIn inData) {
	const int inf = inData.inf;
	int n = inData.liczbaWierzcholkow;          // liczba wierzcholkow
	//int tablica[100][100];
	KruskalIn::MacierzGrafu tablica = inData.macierzGrafu;
	int tablicaKoncowa[100][100];
	char s[5]; // tablica do znaku "*"
	int licznik = 0; //tajny licznik do tanych misji

	int minimum = 0; // minimalna wartosc w tablicy
	int minPozycja[2];   //pozycja minimalnej wartosci


	listaPom listaPomocnicza;       //wypelnienie listy sprawdzajacej numerami wierzcholkow
	while (licznik < n) {
		listaPomocnicza.dodaj(licznik);
		licznik++;
	}

	liczba *root = NULL, *nowa = NULL;
	liczba stosik;
	licznik = 0;                    //zerowanie tajnego licznika
	while (minimum < inf) {
		minimum = tablica[0][0]; // minimum na 1 el tablicy
		for (int i = 0; i < n; i++) {           // Szukanie minimum
			for (int j = 0; j < n; j++) {
				if (minimum == 0) {
					tablica[i][j] = inf;
					tablica[j][i] = inf;
				}
				if (minimum > tablica[i][j]) {
					minimum = tablica[i][j];
					minPozycja[0] = i;            //przypisanie numeru pozycji x
					minPozycja[1] = j;            //przypisanie numeru pozycji y
				}
			}
		}

		if ((listaPomocnicza.znajdz(minPozycja[0], minPozycja[1])) != 0) { //gdy elementy listy nie naleza da tego samego poddrzewa i na stosie jest < n-1 wierzcholkow
			dodaj(&root, minPozycja[0], minPozycja[1], minimum);      // odaj pozycje [x,y] na stos i ich wartosc minimum
			listaPomocnicza.zamien(minPozycja[1], minPozycja[0]);
			tablica[minPozycja[0]][minPozycja[1]] = inf;        // wypelnij ich wartosci inf by nie byly minimum
			tablica[minPozycja[1]][minPozycja[0]] = inf;
		}
		else {
			tablica[minPozycja[0]][minPozycja[1]] = inf;          //gdy sa w tym samym poddrzewie wypelnij ich [x,y] inf by nie byly minimum
			tablica[minPozycja[1]][minPozycja[0]] = inf;
		}

		++licznik;
	}
	/// wypelnienie tablicy koncowej zerami    
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			tablicaKoncowa[i][j] = 0;
		}
	}
	liczba *p = root;
	int sUS = 0;
	while (p) {
		sUS += p->wartosc;
		p = p->next;
	}

	licznik = 0;
	// Uzupelnianie tablicy koncowej 

	p = root;
	while (p) {
		tablicaKoncowa[p->zmienna2][p->zmienna1] = p->wartosc;
		tablicaKoncowa[p->zmienna1][p->zmienna2] = p->wartosc;
		p = p->next;
	}


	KruskalOut outData;
	outData.poprawnyWynik = true;
	outData.dlugoscDrogi = sUS;

	outData.maicerzWyniku.resize(n);
	for (auto &m : outData.maicerzWyniku) m.resize(n);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			outData.maicerzWyniku[i][j] = tablicaKoncowa[i][j];
		}
	}

	return outData;
}

KruskalOut kruskalMaksymalne(KruskalIn inData) {
	const int inf = -inData.inf;

	int n = inData.liczbaWierzcholkow;          // liczba wierzcholkow
	//int tablica[100][100];
	KruskalIn::MacierzGrafu tablica = inData.macierzGrafu;
	int tablicaKoncowa[100][100];
	char s[5]; // tablica do znaku "*"
	int licznik = 0; //tajny licznik do tanych misji

	int minimum = 0; // minimalna wartosc w tablicy
	int minPozycja[2];   //pozycja minimalnej wartosci


	listaPom listaPomocnicza;       //wypelnienie listy sprawdzajacej numerami wierzcholkow
	while (licznik < n) {
		listaPomocnicza.dodaj(licznik);
		licznik++;
	}

	liczba *root = NULL, *nowa = NULL;
	liczba stosik;
	licznik = 0;                    //zerowanie tajnego licznika
	while (minimum > inf) {
		minimum = tablica[0][0]; // minimum na 1 el tablicy
		for (int i = 0; i < n; i++) {           // Szukanie minimum
			for (int j = 0; j < n; j++) {
				if (minimum == 0) {
					tablica[i][j] = inf;
					tablica[j][i] = inf;
				}
				if (minimum < tablica[i][j]) {
					minimum = tablica[i][j];
					minPozycja[0] = i;            //przypisanie numeru pozycji x
					minPozycja[1] = j;            //przypisanie numeru pozycji y
				}
			}
		}

		if ((listaPomocnicza.znajdz(minPozycja[0], minPozycja[1])) != 0) { //gdy elementy listy nie naleza da tego samego poddrzewa i na stosie jest < n-1 wierzcholkow
			dodaj(&root, minPozycja[0], minPozycja[1], minimum);      // odaj pozycje [x,y] na stos i ich wartosc minimum
			listaPomocnicza.zamien(minPozycja[1], minPozycja[0]);
			tablica[minPozycja[0]][minPozycja[1]] = inf;        // wypelnij ich wartosci inf by nie byly minimum
			tablica[minPozycja[1]][minPozycja[0]] = inf;
		}
		else {
			tablica[minPozycja[0]][minPozycja[1]] = inf;          //gdy sa w tym samym poddrzewie wypelnij ich [x,y] inf by nie byly minimum
			tablica[minPozycja[1]][minPozycja[0]] = inf;
		}

		++licznik;
	}
	/// wypelnienie tablicy koncowej zerami    
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			tablicaKoncowa[i][j] = 0;
		}
	}
	liczba *p = root;
	int sUS = 0;
	while (p) {
		sUS += p->wartosc;
		p = p->next;
	}


	licznik = 0;
	// Uzupelnianie tablicy koncowej 

	p = root;
	while (p) {
		tablicaKoncowa[p->zmienna2][p->zmienna1] = p->wartosc;
		tablicaKoncowa[p->zmienna1][p->zmienna2] = p->wartosc;
		p = p->next;
	}


	KruskalOut outData;
	outData.poprawnyWynik = true;
	outData.dlugoscDrogi = sUS;

	outData.maicerzWyniku.resize(n);
	for (auto &m : outData.maicerzWyniku) m.resize(n);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			outData.maicerzWyniku[i][j] = tablicaKoncowa[i][j];
		}
	}

	return outData;
}
