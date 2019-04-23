#include "Narzedzia.h"

void Narzedzia::KonfigurujPrzycisk(
	Fl_Button &przycisk,
	const char *podpowiedz,
	Fl_Pixmap *ikona,
	Fl_Callback *callback,
	void *dane)
{
	przycisk.clear_visible_focus();

	if (podpowiedz) przycisk.tooltip(podpowiedz);
	if (ikona) przycisk.image(ikona);
	if (callback) przycisk.callback(callback, dane);
}

std::array<int, 2> Narzedzia::rozmiarTekstu(std::string tekst, uint32_t rozmiar)
{
	fl_font(FL_HELVETICA_BOLD, rozmiar);
	std::array<int, 2> tab;
	fl_measure(tekst.c_str(), tab[0], tab[1]);
	return tab;
}
