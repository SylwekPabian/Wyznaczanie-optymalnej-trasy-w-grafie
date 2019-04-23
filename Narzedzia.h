#pragma once

#include <FL/Fl_Button.H>
#include <FL/Fl_Pixmap.H>
#include <FL/fl_draw.H>

#include <array>

class Narzedzia
{
public:
	static void KonfigurujPrzycisk(
		Fl_Button &przycisk,
		const char *podpowiedz,
		Fl_Pixmap *ikona,
		Fl_Callback *callback,
		void *dane = nullptr);
	static std::array<int, 2> rozmiarTekstu(
		std::string tekst,
		uint32_t rozmiar);
};