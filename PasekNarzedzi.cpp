#include "PasekNarzedzi.h"
#include "Narzedzia.h"
#include "Ikonki.h"
#include "BellmanFord.h"

const uint32_t PasekNarzedzi::RozmiarPrzycisku = 40;
const uint32_t PasekNarzedzi::SzerokoscSeparatora = 5;
const uint32_t PasekNarzedzi::SzerokoscWyboruAlgorytmu = 200;

void nowyGrafCallback(Fl_Widget* widget, void*)
{
	PasekNarzedzi *pasekNarzedzi = static_cast<PasekNarzedzi *>(widget->parent());
	if (pasekNarzedzi->grafWidget->graf.wezly.empty()
		|| fl_ask("Aktualny graf zostanie zast¹piony. Czy chcesz kontynuowaæ?"))
	{
		pasekNarzedzi->grafWidget->graf.wezly.clear();
	}
	pasekNarzedzi->grafWidget->redraw();
}

void otworzGrafCallback(Fl_Widget *widget, void*)
{
	PasekNarzedzi *pasekNarzedzi = static_cast<PasekNarzedzi *>(widget->parent());
	pasekNarzedzi->grafWidget->wczytajZPliku();
	pasekNarzedzi->grafWidget->redraw();
}

void zapiszGrafCallback(Fl_Widget *widget, void*)
{
	PasekNarzedzi *pasekNarzedzi = static_cast<PasekNarzedzi *>(widget->parent());
	pasekNarzedzi->grafWidget->zapiszDoPliku();
}

void trybUsuwaniaCallback(Fl_Widget *widget, void*)
{
	PasekNarzedzi *pasekNarzedzi = static_cast<PasekNarzedzi *>(widget->parent());
	pasekNarzedzi->grafWidget->zmienTrybEdycji();
}

void selectCallback(Fl_Widget *widget, void*)
{
	PasekNarzedzi *pasekNarzedzi = static_cast<PasekNarzedzi *>(widget->parent());
	pasekNarzedzi->grafWidget->graf.wyczyscZaznaczenie();
	pasekNarzedzi->grafWidget->redraw();
}

void startCallback(Fl_Widget *widget, void*)
{
	PasekNarzedzi *pasekNarzedzi = static_cast<PasekNarzedzi *>(widget->parent());
	pasekNarzedzi->grafWidget->graf.wyczyscZaznaczenie();

	if (pasekNarzedzi->grafWidget->graf.wezly.empty()) return;

	switch (pasekNarzedzi->wyborAlgorytmu.value())
	{
	case 0: { // Bellman-Ford
		BellmanFordOut wynikBellman;
		wynikBellman = bellmanFord(pasekNarzedzi->grafWidget->graf.daneDoBellmanaForda());

		if (wynikBellman.poprawnyWynik)
		{
			pasekNarzedzi->grafWidget->graf.zaznaczNaGrafie(wynikBellman);
		}

		} break;

	case 1: { // Dantzig
		DantzigOut wynikDantzig;
		wynikDantzig = dantzig(pasekNarzedzi->grafWidget->graf.daneDoDantziga());

		if (wynikDantzig.poprawnyWynik)
		{
			pasekNarzedzi->grafWidget->graf.zaznaczNaGrafie(wynikDantzig);
		}

		} break;

	case 2: { // Kruskal Min
		KruskalOut wynikKruskalMin;
		wynikKruskalMin = kruskalMinimalne(pasekNarzedzi->grafWidget->graf.daneDoKruskala());

		if (wynikKruskalMin.poprawnyWynik)
		{
			pasekNarzedzi->grafWidget->graf.zaznaczNaGrafie(wynikKruskalMin);
		}

	} break;

	case 3: { // Kruskal Max
		KruskalOut wynikKruskalMax;
		wynikKruskalMax = kruskalMaksymalne(pasekNarzedzi->grafWidget->graf.daneDoKruskalaMax());

		if (wynikKruskalMax.poprawnyWynik)
		{
			pasekNarzedzi->grafWidget->graf.zaznaczNaGrafie(wynikKruskalMax);
		}
	} break;

	}

	pasekNarzedzi->grafWidget->redraw();
}

PasekNarzedzi::PasekNarzedzi(GrafWidget *grafWidget_, int x, int y, int szerokosc, int wysokosc)
	: Fl_Pack(x, y, szerokosc, wysokosc)
	, grafWidget(grafWidget_)
	, nowyGraf(0, 0, RozmiarPrzycisku, RozmiarPrzycisku)
	, otworzGraf(0, 0, RozmiarPrzycisku, RozmiarPrzycisku)
	, zapiszGraf(0, 0, RozmiarPrzycisku, RozmiarPrzycisku)
	, separator1(0, 0, SzerokoscSeparatora, RozmiarPrzycisku)
	, trybUsuwania(0, 0, RozmiarPrzycisku, RozmiarPrzycisku)
	, separator2(0, 0, SzerokoscSeparatora, RozmiarPrzycisku)
	, wyborAlgorytmu(0, 0, SzerokoscWyboruAlgorytmu, RozmiarPrzycisku)
	, uruchomAlgorytm(0, 0, RozmiarPrzycisku, RozmiarPrzycisku)
{
	type(Fl_Pack::HORIZONTAL);
	spacing(4);

	Narzedzia::KonfigurujPrzycisk(nowyGraf, "Nowy graf", new Fl_Pixmap(Ikonki::nowy), nowyGrafCallback);
	Narzedzia::KonfigurujPrzycisk(otworzGraf, "Wczytaj graf", new Fl_Pixmap(Ikonki::otworz), otworzGrafCallback);
	Narzedzia::KonfigurujPrzycisk(zapiszGraf, "Zapisz graf", new Fl_Pixmap(Ikonki::zapisz), zapiszGrafCallback);
	Narzedzia::KonfigurujPrzycisk(uruchomAlgorytm, "Start", new Fl_Pixmap(Ikonki::start), startCallback);

	separator1.box(FL_UP_FRAME);

	Narzedzia::KonfigurujPrzycisk(trybUsuwania, "Tryb usuwania", new Fl_Pixmap(Ikonki::usuwanie), trybUsuwaniaCallback);
	trybUsuwania.selection_color(FL_RED);
	trybUsuwania.type(FL_TOGGLE_BUTTON);

	separator2.box(FL_UP_FRAME);

	wyborAlgorytmu.add("Bellman-Ford");
	wyborAlgorytmu.add("Dantzig");
	wyborAlgorytmu.add("Kruskal Minimalne");
	wyborAlgorytmu.add("Kruskal Maksymalne");
	wyborAlgorytmu.value(0);
	wyborAlgorytmu.callback(selectCallback);

	end();
}