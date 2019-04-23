#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <list>

#include "Dantzig.h"

#define inf 100000
using namespace std;

struct listaWierzcholkow {   //wierzcholki wykorzystane
	int wierzcholek;
	listaWierzcholkow *head;
	listaWierzcholkow *next;
	listaWierzcholkow();

	void dodaj(int);
	bool sprawdz(int);
	bool znajdz(int, int);
	bool znajdzJeden(int);
};

listaWierzcholkow::listaWierzcholkow() {
	head = 0;
	next = 0;
};

bool listaWierzcholkow::sprawdz(int w) {
	listaWierzcholkow *temp = head;
	while (temp) {
		if (temp->wierzcholek == w)
			return true;
		temp = temp->next;
	}
	return false;
};

void listaWierzcholkow::dodaj(int wierzch) {
	if (sprawdz(wierzch)) return;
	else {
		listaWierzcholkow  *nowa = new listaWierzcholkow;

		nowa->wierzcholek = wierzch;

		if (head == 0) {
			head = nowa;
		}
		else {
			listaWierzcholkow *temp = head;
			while (temp->next) {
				if (temp->wierzcholek == wierzch)
					return;
				temp = temp->next;
			}
			temp->next = nowa;
			nowa->next = 0;
		}
	}
};

bool listaWierzcholkow::znajdz(int w, int y) {
	listaWierzcholkow *temp = head;
	int prawdaDoDwuch = 0;
	while (temp) {
		if (w == temp->wierzcholek || y == temp->wierzcholek) {
			++prawdaDoDwuch;
			if (prawdaDoDwuch == 2) {
				return true;
			}
		}
		temp = temp->next;
	}
	return false;

};

bool listaWierzcholkow::znajdzJeden(int w) {
	listaWierzcholkow *temp = head;
	while (temp) {
		if (w == temp->wierzcholek) {
			return true;
		}
		temp = temp->next;
	}
	return false;

};

struct lista {            // struktura listy minimum
	int x;
	int y;
	int *wskaznik;		//wskaznik do wierzcholka
	int suma;
	lista *head;         // poczatek
	lista *next;         // kolejny el struktury
	lista();

	void dodaj(int, int, int *, int);
	int minListy();
	void usunMin();
	void usunWykorzystane(listaWierzcholkow);

};
lista::lista() {
	head = 0;
	next = 0;
};

void lista::dodaj(int wx, int wy, int *wskaz, int s) {
	lista *nowa = new lista;
	nowa->x = wx;
	nowa->y = wy;
	nowa->wskaznik = wskaz;
	nowa->suma = s;

	if (head == 0) {
		head = nowa;
	}
	else {
		lista *temp = head;
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = nowa;
		nowa->next = 0;
	}
};

int lista::minListy() {
	lista *temp = head;
	if (head == 0) {
		return 0;
	}
	int min = temp->suma;
	while (temp) {
		if (min > temp->suma) {
			min = temp->suma;
		}
		temp = temp->next;
	}

	temp = head;
	int x, y, su;
	while ((min != temp->suma)) {
		temp = temp->next;
	}
	x = head->x;		// zamiana miejscami minimum na 1pozycje (head)
	y = head->y;
	su = head->suma;

	head->x = temp->x;
	head->y = temp->y;
	head->suma = temp->suma;

	temp->x = x;
	temp->y = y;
	temp->suma = su;

	return min;
};
void lista::usunWykorzystane(listaWierzcholkow wykorzystane) {
	lista *temp = head;
	lista *poprzedni = head;

	while (temp) {
		if (wykorzystane.znajdzJeden(temp->x)) {
			if (temp == head) {
				head = head->next;
				delete temp;
				temp = head;
				poprzedni = head;
			}
			else {
				poprzedni->next = temp->next;
				delete temp;
				temp = poprzedni->next;
			}
		}
		else {
			poprzedni = temp;
			temp = temp->next;
		}
	}
};

bool pusty(int *p, int n) {	 // sprawdza czy wszystkei drogi od wierzcholka byly juz uwzglednione
	for (int i = 0; i < n; i++) {
		if (*(p++) != inf) {
			return false;
		}
	}
	return true;
};

int minimum(int *p, int *SumaW, int n) {		// szuka minimum z wierzcholka
	int min = *p;
	int pozycja = 0;

	for (int i = 0; i < n; i++) {
		if (min > *p) {
			min = *p;
			pozycja = i;
		}
		*(p++);
	}
	*SumaW = min;
	return pozycja;		// zwraca pozycje na ktorej jesst minimum w tabilcy
};

int Wartoscminimum(int *p, int n) {		// szuka minimum z wierzcholka
	int min = *p;

	for (int i = 0; i < n; i++) {
		if (min > *p) {
			min = *p;
		}
		*(p++);
	}
	return min;		// zwraca  minimum w tabilcy
};

DantzigOut dantzig(DantzigIn inData) {

	int n = inData.liczbaWierzcholkow;	// zmienan przechowuje ilosc wierzcholkow 1linia z pliku
	//int tablica[100][100];
	DantzigIn::MacierzGrafu tablica = inData.macierzGrafu;

	int tablicaKoncowa[100][100];
	char s[5]; // tablica do znaku "*"
	int licznik = 0; //tajny licznik do tanych misji

	/// wypelnienie tablicy koncowej tablica poczatkowa    
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			tablicaKoncowa[i][j] = 0;
		}
	}

	int sumaSciezki = 0;
	int sumaWierzcholka = 0;
	int *wskSumaWierzcholka;
	wskSumaWierzcholka = &sumaWierzcholka;

	lista listaWierzcholkowMinimum;
	listaWierzcholkowMinimum.dodaj(0, 0, &tablica[0][0], 0);

	int numerPoprzedniego = 0;
	int numerNastepnego = 0;

	listaWierzcholkow listaWierzcholkowSpr;
	listaWierzcholkowSpr.dodaj(0);

	int *wierzcholekNastepny;	// wsk na nast wybrany wierzcholek
	wierzcholekNastepny = &tablica[0][0];

	listaWierzcholkowMinimum.minListy();
	int alaa = 1;
	while (listaWierzcholkowMinimum.head->wskaznik != &tablica[n - 1][0]) {

		listaWierzcholkowSpr.dodaj(listaWierzcholkowMinimum.head->x);

		while (!pusty(listaWierzcholkowMinimum.head->wskaznik, n) && licznik < n) {

			numerNastepnego = minimum(listaWierzcholkowMinimum.head->wskaznik, wskSumaWierzcholka, n);
			numerPoprzedniego = listaWierzcholkowMinimum.head->x;

			sumaSciezki = listaWierzcholkowMinimum.head->suma;
			sumaSciezki += Wartoscminimum(listaWierzcholkowMinimum.head->wskaznik, n);

			wierzcholekNastepny = &tablica[numerNastepnego][0];
			if (!listaWierzcholkowSpr.znajdzJeden(numerNastepnego))
				listaWierzcholkowMinimum.dodaj(numerNastepnego, numerPoprzedniego, wierzcholekNastepny, sumaSciezki);

			tablica[numerPoprzedniego][numerNastepnego] = inf;
			tablica[numerNastepnego][numerPoprzedniego] = inf;

			++licznik;
		}
		licznik = 0;
		listaWierzcholkowMinimum.usunWykorzystane(listaWierzcholkowSpr);

		listaWierzcholkowMinimum.minListy();
		sumaSciezki = listaWierzcholkowMinimum.head->suma;
		tablicaKoncowa[listaWierzcholkowMinimum.head->y][listaWierzcholkowMinimum.head->x] = alaa;
		alaa++;

	}

	// wprowadzenie do talbicy koncowej wartosci z grafu ktore sa w sciezce	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (tablicaKoncowa[i][j] == tablica[i][j]) {
				tablicaKoncowa[i][j] = 0;
			}
		}
	}

	DantzigOut outData;
	outData.poprawnyWynik = true;
	outData.dlugoscDrogi = sumaSciezki;
	outData.maicerzWyniku.resize(n);
	for (auto &m : outData.maicerzWyniku) m.resize(n);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			outData.maicerzWyniku[i][j] = tablicaKoncowa[i][j];
		}
	}

	return outData;
}