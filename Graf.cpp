#include <algorithm>
#include <iomanip>

#include "Graf.h"
#include "Narzedzia.h"

const uint8_t Graf::rozmiarWezla = 17;
const uint8_t Graf::rozmiarCzcionkiDrogi = 12;

Graf::Graf()
	: nastepnyIndeks(1)
	, aktywneZaznaczenie(false)
	, najkrotszaDroga(INT32_MAX)
{
}

void Graf::dodajWezel(uint32_t x, uint32_t y)
{
	wyczyscZaznaczenie();

	Wezel nowyWezel;
	nowyWezel.x = x;
	nowyWezel.y = y;
	wezly.push_back(nowyWezel);
	for (auto &wezel : wezly)
	{
		wezel.drogi.resize(wezly.size());
	}
}

uint32_t Graf::znajdzWezel(uint32_t x, uint32_t y)
{
	for (uint32_t i = 0; i < wezly.size(); i++)
	{
		if (x >= (wezly[i].x - rozmiarWezla) && x <= (wezly[i].x + rozmiarWezla)
			&& y >= (wezly[i].y - rozmiarWezla) && y <= (wezly[i].y + rozmiarWezla))
		{
			return i;
		}
	}
	return UINT32_MAX;
}

Graf::Droga* Graf::znajdzDroge(uint32_t x, uint32_t y)
{
	for (auto &wezel : wezly)
	{
		for (auto &droga : wezel.drogi)
		{
			if (droga.waga == INT32_MAX) continue;

			if (x >= (droga.x - (droga.szerokosc / 2)) && x <= (droga.x + (droga.szerokosc / 2))
				&& y >= (droga.y - (droga.wysokosc / 2)) && y <= (droga.y + (droga.wysokosc / 2)))
			{
				return &droga;
			}
		}
	}

	return nullptr;
}

Graf::Droga* Graf::dodajDroge(uint32_t indeksPoczatkowy, uint32_t indeksKoncowy, uint32_t waga)
{
	if (indeksPoczatkowy != UINT32_MAX && indeksKoncowy != UINT32_MAX
		&& indeksPoczatkowy != indeksKoncowy // bez polaczen sam do siebie
		&& wezly[indeksKoncowy].drogi[indeksPoczatkowy].waga == INT32_MAX // nie laczyc, jesli juz byla droga w druga strone
		&& wezly[indeksPoczatkowy].drogi[indeksKoncowy].waga == INT32_MAX) // i nie przeciagac drugi raz tgo samego
	{
		wyczyscZaznaczenie();

		Graf::Wezel &wezelPoczatkowy = wezly[indeksPoczatkowy];
		Graf::Wezel &wezelKoncowy = wezly[indeksKoncowy];
		
		wezelPoczatkowy.drogi[indeksKoncowy].x = (wezelPoczatkowy.x + wezelKoncowy.x) / 2;
		wezelPoczatkowy.drogi[indeksKoncowy].y = (wezelPoczatkowy.y + wezelKoncowy.y) / 2;
		
		return &wezelPoczatkowy.drogi[indeksKoncowy];
	}
	return nullptr;
}

void Graf::zmienWageDrogi(Graf::Droga &droga, int32_t waga)
{
	droga.waga = waga;
	std::array<int, 2> tab = Narzedzia::rozmiarTekstu(std::to_string(waga), Graf::rozmiarCzcionkiDrogi);
	droga.szerokosc = tab[0] + 5;
	droga.wysokosc = tab[1] + 5;
}

void Graf::usunWezel(uint32_t indeksWezla)
{
	if (indeksWezla != UINT32_MAX)
	{
		for (auto &wezel : wezly)
		{
			wezel.drogi.erase(wezel.drogi.begin() + indeksWezla);
		}
		wezly.erase(wezly.begin() + indeksWezla);

		wyczyscZaznaczenie();
	}
}

void Graf::usunDroge(uint32_t x, uint32_t y)
{
	for (auto &wezel : wezly)
	{
		for (uint32_t i = 0; i < wezel.drogi.size(); i++)
		{
			if (x >= (wezel.drogi[i].x - rozmiarCzcionkiDrogi) && x <= (wezel.drogi[i].x + rozmiarCzcionkiDrogi)
				&& y >= (wezel.drogi[i].y - rozmiarCzcionkiDrogi) && y <= (wezel.drogi[i].y + rozmiarCzcionkiDrogi))
			{
				wyczyscZaznaczenie();

				wezel.drogi[i].waga = INT32_MAX;
				wezel.drogi[i].x = UINT32_MAX;
				wezel.drogi[i].y = UINT32_MAX;
				return;
			}
		}
	}
}

void Graf::zaznaczNaGrafie(BellmanFordOut daneDoZaznaczenia)
{
	aktywneZaznaczenie = true;
	najkrotszaDroga = daneDoZaznaczenia.najkrotszaDroga;
	wezly[0].zaznaczenie = Graf::OPTYMALNE;

	for (uint32_t i = 1; i < daneDoZaznaczenia.droga.size(); ++i)
	{
		wezly[daneDoZaznaczenia.droga[i - 1]].drogi[daneDoZaznaczenia.droga[i]].zaznaczenie = Graf::OPTYMALNE;
		wezly[daneDoZaznaczenia.droga[i]].zaznaczenie = Graf::OPTYMALNE;
	}

	wygenerujTekstWyniku(daneDoZaznaczenia);
}

void Graf::wygenerujTekstWyniku(BellmanFordOut daneDoZaznaczenia)
{
	std::stringstream wynik;

	wynik << "---------------------------------------------------------------\n";
	wynik << "Obliczony koszt drogi:   " << daneDoZaznaczenia.najkrotszaDroga << "\n";
	wynik << "---------------------------------------------------------------\n\n\n";

	uint32_t elementyTablicy = daneDoZaznaczenia.D.size();

	for (uint32_t i = 0; i <= elementyTablicy; ++i) wynik << std::left << std::setw(10) << std::setfill('-') << "+";
	wynik << "+\n|";
	for (uint32_t i = 0; i <= elementyTablicy; ++i)
		wynik << std::left << std::setw(8) << std::setfill(' ') << (i != 0 ? std::to_string(i) : "") << " |";
	wynik << "\n";
	for (uint32_t i = 0; i <= elementyTablicy; ++i) wynik << std::left << std::setw(10) << std::setfill('-') << "+";

	// D
	wynik << "+\n|";
	wynik << std::right << std::setw(10) << std::setfill(' ') << "D |";
	for (uint32_t i = 0; i < elementyTablicy; ++i)
		wynik << std::right << std::setw(8) << std::setfill(' ') << daneDoZaznaczenia.D[i] << " |";

	// P
	wynik << "\n";
	for (uint32_t i = 0; i <= elementyTablicy; ++i) wynik << std::left << std::setw(10) << std::setfill('-') << "+";
	wynik << "+\n|";
	wynik << std::right << std::setw(10) << std::setfill(' ') << "P |";
	for (uint32_t i = 0; i < elementyTablicy; ++i)
		wynik << std::right << std::setw(8) << std::setfill(' ') << daneDoZaznaczenia.p[i] << " |";

	wynik << "\n";
	for (uint32_t i = 0; i <= elementyTablicy; ++i) wynik << std::left << std::setw(10) << std::setfill('-') << "+";
	wynik << "+\n";

	wynikAlgorytmuTxt = wynik.str();
}

Graf::Zaznaczenie Graf::danzingZaznaczanieRek(DantzigOut & daneDoZaznaczenia, uint32_t wezel, uint32_t poprzedni)
{
	DantzigOut::Wynik& wynik = daneDoZaznaczenia.maicerzWyniku;
	uint32_t ostatniWezel = wynik.size() - 1;

	bool pusto = true;

	for (uint32_t i = 0; i < wynik[wezel].size(); ++i)
	{
		if (wynik[wezel][i] != 0)
		{
			int32_t cel = i; //wynik[wezel][i];
			pusto = false;
			Graf::Zaznaczenie zaznaczenie = danzingZaznaczanieRek(daneDoZaznaczenia, cel);
			if (zaznaczenie == Graf::OPTYMALNE)
			{
				wezly[wezel].zaznaczenie = Graf::OPTYMALNE;
				wezly[wezel].drogi[cel].zaznaczenie = Graf::OPTYMALNE;
			}
			else if (zaznaczenie == Graf::PODSWIETL_CZERWONY)
			{
				if (wezly[wezel].zaznaczenie != Graf::OPTYMALNE)
					wezly[wezel].zaznaczenie = Graf::PODSWIETL_CZERWONY;
				if (wezly[wezel].drogi[cel].zaznaczenie != Graf::OPTYMALNE)
					wezly[wezel].drogi[cel].zaznaczenie = Graf::PODSWIETL_CZERWONY;
			}
		}
	}

	if (pusto)
	{
		if (wezel == ostatniWezel)
		{
			wezly[wezel].zaznaczenie = Graf::OPTYMALNE;
			return Graf::OPTYMALNE;
		}
		else
		{
			if (wezly[wezel].zaznaczenie != Graf::OPTYMALNE)
				wezly[wezel].zaznaczenie = Graf::PODSWIETL_CZERWONY;
			return Graf::PODSWIETL_CZERWONY;
		}
	}

	return wezly[wezel].zaznaczenie;
}

void Graf::zaznaczNaGrafie(DantzigOut daneDoZaznaczenia)
{
	aktywneZaznaczenie = true;
	najkrotszaDroga = daneDoZaznaczenia.dlugoscDrogi;
	danzingZaznaczanieRek(daneDoZaznaczenia, 0);

	wygenerujTekstWyniku(daneDoZaznaczenia);
}

void Graf::wygenerujTekstWyniku(DantzigOut daneDoZaznaczenia)
{
	std::stringstream wynik;

	wynik << "---------------------------------------------------------------\n";
	wynik << "Obliczony koszt drogi:   " << daneDoZaznaczenia.dlugoscDrogi << "\n";
	wynik << "---------------------------------------------------------------\n";

	// nie wypisujemy zer z macierzy wyniku
	wynik << generujMacierzTxt(daneDoZaznaczenia.maicerzWyniku, "", 0);

	wynikAlgorytmuTxt = wynik.str();
}

Graf::Zaznaczenie Graf::kruskalZaznaczanieRek(KruskalOut & daneDoZaznaczenia, uint32_t wezel, uint32_t poprzedni)
{
	KruskalOut::Wynik& wynik = daneDoZaznaczenia.maicerzWyniku;
	uint32_t ostatniWezel = wynik.size() - 1;

	bool nastepny = false;
	for (uint32_t i = 0; i < wynik[wezel].size(); ++i)
	{
		if (wynik[wezel][i] != 0 && i != poprzedni && wezel != ostatniWezel)
		{
			nastepny = true;
			uint32_t zaznacznie = kruskalZaznaczanieRek(daneDoZaznaczenia, i, wezel);
			if (zaznacznie == Graf::OPTYMALNE)
			{
				wezly[wezel].zaznaczenie = Graf::OPTYMALNE;
				wezly[wezel].drogi[i].zaznaczenie = Graf::OPTYMALNE;
			}
			else if (zaznacznie == Graf::PODSWIETL_CZERWONY)
			{
				if (wezly[wezel].zaznaczenie != Graf::OPTYMALNE)
					wezly[wezel].zaznaczenie = Graf::PODSWIETL_CZERWONY;
				if (poprzedni != INT32_MAX && wezly[wezel].drogi[i].zaznaczenie != Graf::OPTYMALNE)
					wezly[wezel].drogi[i].zaznaczenie = Graf::PODSWIETL_CZERWONY;
				//TODO: wezly od tylu?
				if (poprzedni != INT32_MAX && wezly[i].drogi[wezel].zaznaczenie != Graf::OPTYMALNE)
					wezly[i].drogi[wezel].zaznaczenie = Graf::PODSWIETL_CZERWONY;
			}
		}
	}

	if (!nastepny)
	{
		if (wezel == ostatniWezel)
		{
			wezly[wezel].zaznaczenie = Graf::OPTYMALNE;
			return Graf::OPTYMALNE;
		}
		else
		{
			if (wezly[wezel].zaznaczenie != Graf::OPTYMALNE)
				wezly[wezel].zaznaczenie = Graf::PODSWIETL_CZERWONY;
			return Graf::PODSWIETL_CZERWONY;
		}
	}

	return wezly[wezel].zaznaczenie;
}

void Graf::zaznaczNaGrafie(KruskalOut daneDoZaznaczenia)
{
	aktywneZaznaczenie = true;
	najkrotszaDroga = daneDoZaznaczenia.dlugoscDrogi;
	kruskalZaznaczanieRek(daneDoZaznaczenia, 0);

	wygenerujTekstWyniku(daneDoZaznaczenia);
}

void Graf::wygenerujTekstWyniku(KruskalOut daneDoZaznaczenia)
{
	std::stringstream wynik;

	wynik << "---------------------------------------------------------------\n";
	wynik << "Obliczony koszt drogi:   " << daneDoZaznaczenia.dlugoscDrogi << "\n";
	wynik << "---------------------------------------------------------------\n";

	// nie wypisujemy zer z macierzy wyniku
	wynik << generujMacierzTxt(daneDoZaznaczenia.maicerzWyniku, "", 0);

	wynikAlgorytmuTxt = wynik.str();
}

void Graf::wyczyscZaznaczenie()
{
	for (auto &wezel : wezly)
	{
		wezel.zaznaczenie = Graf::BRAK;
		for (auto &droga : wezel.drogi)
		{
			droga.zaznaczenie = Graf::BRAK;
		}
	}
	najkrotszaDroga = UINT32_MAX;
	aktywneZaznaczenie = false;

	wynikAlgorytmuTxt.clear();
}

std::string Graf::generujMacierzTxt(std::vector<std::vector<int32_t>> &macierz, std::string wypelniacz, int32_t wartoscDoZastapienia)
{
	std::stringstream wynik;
	uint32_t szerokosc = macierz.size();

	wynik << "\n\n";
	for (uint32_t i = 0; i <= szerokosc; ++i) wynik << std::left << std::setw(10) << std::setfill('-') << "+";
	wynik << "+\n|";
	for (uint32_t i = 0; i <= szerokosc; ++i)
		wynik << std::left << std::setw(8) << std::setfill(' ') << (i != 0 ? std::to_string(i) : "") << " |";
	wynik << "\n";
	for (uint32_t i = 0; i <= szerokosc; ++i) wynik << std::left << std::setw(10) << std::setfill('-') << "+";
	wynik << "+";

	for (uint32_t i = 0; i < macierz.size(); ++i)
	{
		wynik << "\n|";
		wynik << std::right << std::setw(10) << std::setfill(' ') << std::to_string(i + 1) + " |";
		for (uint32_t j = 0; j < szerokosc; ++j)
		{
			std::string wartosc = macierz[i][j] != wartoscDoZastapienia ? std::to_string(macierz[i][j]) : wypelniacz;
			wynik << std::right << std::setw(8) << std::setfill(' ') << wartosc << " |";
		}
		wynik << "\n";
		for (uint32_t i = 0; i <= szerokosc; ++i) wynik << std::left << std::setw(10) << std::setfill('-') << "+";
		wynik << "+";
	}

	return wynik.str();
}

BellmanFordIn Graf::daneDoBellmanaForda()
{
	BellmanFordIn dane;

	dane.wierzcholekStartowy = 0;
	dane.liczbaWierzcholkow = wezly.size();

	for (const auto &wezel : wezly)
	{
		std::vector<BellmanFordIn::Krawedz> listaSasiadowWezla;
		for (uint32_t i = 0; i < wezel.drogi.size(); ++i)
		{
			if (wezel.drogi[i].waga != INT32_MAX) listaSasiadowWezla.push_back(BellmanFordIn::Krawedz(i, wezel.drogi[i].waga));
		}
		dane.listaSasiadow.push_back(listaSasiadowWezla);
	}

	return dane;
}

DantzigIn Graf::daneDoDantziga()
{
	DantzigIn dane;

	dane.wierzcholekStartowy = 0;
	dane.liczbaWierzcholkow = wezly.size();
	
	for (const auto& wezel : wezly)
	{
		std::vector<int> wezelVec;

		for (const auto& droga : wezel.drogi)
		{
			if (droga.waga != INT32_MAX) wezelVec.push_back(droga.waga);
			else wezelVec.push_back(dane.inf);
		}

		dane.macierzGrafu.push_back(wezelVec);
	}

	return dane;
}

KruskalIn Graf::daneDoKruskala()
{
	KruskalIn dane;

	dane.wierzcholekStartowy = 0;
	dane.liczbaWierzcholkow = wezly.size();

	for (const auto& wezel : wezly)
	{
		std::vector<int> wezelVec;

		for (const auto& droga : wezel.drogi)
		{
			if (droga.waga != INT32_MAX) wezelVec.push_back(droga.waga);
			else wezelVec.push_back(dane.inf);
		}

		dane.macierzGrafu.push_back(wezelVec);
	}

	return dane;
}

KruskalIn Graf::daneDoKruskalaMax()
{
	KruskalIn dane;

	dane.wierzcholekStartowy = 0;
	dane.liczbaWierzcholkow = wezly.size();

	for (const auto& wezel : wezly)
	{
		std::vector<int> wezelVec;

		for (const auto& droga : wezel.drogi)
		{
			if (droga.waga != INT32_MAX) wezelVec.push_back(droga.waga);
			else wezelVec.push_back(-dane.inf);
		}

		dane.macierzGrafu.push_back(wezelVec);
	}

	return dane;
}

std::string Graf::wygenerujMacierzAsoc()
{
	std::vector<std::vector<int32_t>> m(wezly.size());
	for (uint32_t i = 0; i < wezly.size(); ++i)
	{
		for (const auto& droga : wezly[i].drogi)
		{
			if (droga.waga != INT32_MAX) m[i].push_back(1);
			else m[i].push_back(droga.waga);
		}
	}
	return generujMacierzTxt(m, "");
}

std::string Graf::wygenerujMacierzKosz()
{
	std::vector<std::vector<int32_t>> m(wezly.size());
	for (uint32_t i = 0; i < wezly.size(); ++i)
	{
		for (const auto& droga : wezly[i].drogi)
		{
			m[i].push_back(droga.waga);
		}
	}
	return generujMacierzTxt(m, "");
}

std::string Graf::pobierzWynikAlgorytmu()
{
	return wynikAlgorytmuTxt;
}

