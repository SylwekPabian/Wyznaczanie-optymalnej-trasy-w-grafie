#pragma once

#include <FL/Fl_Pack.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>

#include "GrafWidget.h"

class PasekWyboruWidoku : public Fl_Pack
{
public:
	// typy
	enum WybranyWidok { WIDOK_GRAFU, WIDOK_WYNIK, WIDOK_ASOC, WIDOK_KOSZ, WIDOK_AUTORZY };

	// metody
	PasekWyboruWidoku(GrafWidget *grafWidget_, int x, int y, int szerokosc, int wysokosc);

	void ustaw(Fl_Text_Display *wyswietlanieTekstu, Fl_Text_Buffer *buforTekstu);
	void konfigurujPrzyciskRadio(Fl_Button &przycisk);

	// stale
	static const uint32_t WysokoscPrzycisku;
	static const uint32_t SzerokoscPrzycisku;

	// kontrolki
	Fl_Box widokLabel;
	Fl_Button widokGrafu;
	Fl_Button widokWynikAlgorytmu;
	Fl_Button widokMacierzAsoc;
	Fl_Button widokMacierzKosz;
	Fl_Button widokAutorzy;

	// atrybuty
	WybranyWidok wybranyWidok;

	GrafWidget *grafWidget;

	Fl_Text_Display *wyswietlanieTekstu;
	Fl_Text_Buffer *buforTekstu;
};