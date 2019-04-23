#pragma once

#include <FL\Fl.H>
#include <FL\Fl_Double_Window.H>
#include <FL/Fl_Text_Display.H>

#include "GrafWidget.h"
#include "PasekNarzedzi.h"
#include "PasekWyboruWidoku.h"

class Okno : public Fl_Double_Window
{
public:
	// metody
	Okno(int szerokosc, int wysokosc, const char *tytul = nullptr);

	// kontrolki
	Fl_Text_Buffer buforTekstu;
	Fl_Text_Display wyswietlanieTekstu;
	GrafWidget grafWidget;
	PasekNarzedzi pasekNarzedzi;
	PasekWyboruWidoku pasekWyboruWidoku;
};