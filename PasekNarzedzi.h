#pragma once

#include <cstdint>

#include <FL/Fl.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Choice.H>

#include "GrafWidget.h"

class PasekNarzedzi : public Fl_Pack
{
public:
	// metody
	PasekNarzedzi(GrafWidget *grafWidget_, int x, int y, int szerokosc, int wysokosc);

	// stale
	static const uint32_t RozmiarPrzycisku;
	static const uint32_t SzerokoscSeparatora;
	static const uint32_t SzerokoscWyboruAlgorytmu;

	// kontrolki
	Fl_Button nowyGraf;
	Fl_Button otworzGraf;
	Fl_Button zapiszGraf;
	Fl_Box separator1;
	Fl_Button trybUsuwania;
	Fl_Box separator2;
	Fl_Choice wyborAlgorytmu;
	Fl_Button uruchomAlgorytm;

	// atrybuty
	GrafWidget *grafWidget;
};