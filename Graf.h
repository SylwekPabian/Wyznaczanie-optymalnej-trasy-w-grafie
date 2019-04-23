#pragma once

#include <vector>
#include <cstdint>
#include <sstream>

#include "BellmanFord.h"
#include "Dantzig.h"
#include "Kruskal.h"

class Graf
{
public: //Struktury
	enum Zaznaczenie
	{
		BRAK,
		OPTYMALNE,
		PODSWIETL_CZERWONY
	};

	struct Droga
	{
		Droga() : waga(INT32_MAX), zaznaczenie(BRAK) {}

		int32_t waga;
		uint32_t x;
		uint32_t y;
		uint32_t wysokosc;
		uint32_t szerokosc;

		Zaznaczenie zaznaczenie;
	};

	struct Wezel
	{
		Wezel() : zaznaczenie(BRAK) {}

		std::vector<Droga> drogi;
		uint32_t x;
		uint32_t y;

		Zaznaczenie zaznaczenie;
	};

public://Metody
	Graf();
	
	void dodajWezel(uint32_t x, uint32_t y);
	uint32_t znajdzWezel(uint32_t x, uint32_t y);
	Graf::Droga* znajdzDroge(uint32_t x, uint32_t y);
	Droga* dodajDroge(uint32_t startowy, uint32_t koncowy, uint32_t waga);
	void zmienWageDrogi(Graf::Droga &droga, int32_t waga);
	void usunWezel(uint32_t indeksWezla);
	void usunDroge(uint32_t x, uint32_t y);

	void zaznaczNaGrafie(BellmanFordOut daneDoZaznaczenia);
	void wygenerujTekstWyniku(BellmanFordOut daneDoZaznaczenia);

	Graf::Zaznaczenie danzingZaznaczanieRek(DantzigOut& daneDoZaznaczenia, uint32_t wezel, uint32_t poprzedni = UINT32_MAX);
	void zaznaczNaGrafie(DantzigOut daneDoZaznaczenia);
	void wygenerujTekstWyniku(DantzigOut daneDoZaznaczenia);

	Graf::Zaznaczenie kruskalZaznaczanieRek(KruskalOut& daneDoZaznaczenia, uint32_t wezel, uint32_t poprzedni = UINT32_MAX);
	void zaznaczNaGrafie(KruskalOut daneDoZaznaczenia);
	void wygenerujTekstWyniku(KruskalOut daneDoZaznaczenia);

	void wyczyscZaznaczenie();

	std::string generujMacierzTxt(std::vector<std::vector<int32_t>> &macierz
		, std::string wypelniacz = "*"
		, int32_t wartoscDoZastapienia = INT32_MAX);

	BellmanFordIn daneDoBellmanaForda();
	DantzigIn daneDoDantziga();
	KruskalIn daneDoKruskala();
	KruskalIn daneDoKruskalaMax();

	std::string wygenerujMacierzAsoc();
	std::string wygenerujMacierzKosz();
	std::string pobierzWynikAlgorytmu();

	static const uint8_t rozmiarWezla;
	static const uint8_t rozmiarCzcionkiDrogi;

	std::vector<Wezel> wezly;
	uint32_t nastepnyIndeks;

	bool aktywneZaznaczenie;
	int32_t najkrotszaDroga;
	std::string wynikAlgorytmuTxt;
};
