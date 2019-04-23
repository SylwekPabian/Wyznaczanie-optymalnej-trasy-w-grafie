#pragma once

#include <FL/Fl_Group.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Button.H>

#include <cstdint>

#include "Graf.h"

class GrafWidget : public Fl_Group
{
public:
	GrafWidget(int x, int y, int w, int h);

	virtual void draw();
	virtual int handle(int event);

	void rysowanieSzukanieDrogi();
	void rysujDrogi();
	void rysujWezly();
	void pisz(std::string tekst, uint32_t x, uint32_t y, uint32_t rozmiar);
	void rysujStrzalke(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1);

	void pokazPoleEdycjiWagi();

	void zmienTrybEdycji();
	void zapiszDoPliku();
	void wczytajZPliku();

	void usatwWageNowejDrogi(int32_t waga);

	Fl_Color wezelOptymalny;
	Fl_Color wezelCzerwony;
	Fl_Color wezelStart;
	Fl_Color wezelKoniec;


	Graf graf;
	bool prawyWcisniety;
	bool lewyWcisniety;
	uint32_t wybranyWezel;
	uint32_t myszX;
	uint32_t myszY;

	bool trybUsuwania;
	Fl_Int_Input wagaWidget;
	Fl_Button okButtonWidget;
	Graf::Droga* zmienianaDroga;
};
