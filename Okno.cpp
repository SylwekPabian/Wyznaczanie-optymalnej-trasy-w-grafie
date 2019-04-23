#include "Okno.h"

void close_callback(Fl_Widget*, void*) {
	if (Fl::event() == FL_SHORTCUT && Fl::event_key() == FL_Escape)
		return; // ignore Escape
	exit(0);
}

Okno::Okno(int szerokosc, int wysokosc, const char * tytul)
	: Fl_Double_Window(szerokosc, wysokosc, tytul)
	, grafWidget(3, PasekNarzedzi::RozmiarPrzycisku + 6, w() - 6, h() - 12 - PasekNarzedzi::RozmiarPrzycisku - PasekWyboruWidoku::WysokoscPrzycisku)
	, wyswietlanieTekstu(3, PasekNarzedzi::RozmiarPrzycisku + 6, w() - 6, h() - 12 - PasekNarzedzi::RozmiarPrzycisku - PasekWyboruWidoku::WysokoscPrzycisku)
	, pasekNarzedzi(&grafWidget, 3, 3, w(), PasekNarzedzi::RozmiarPrzycisku)
	, pasekWyboruWidoku(&grafWidget, 3, h() - 3 - PasekWyboruWidoku::WysokoscPrzycisku, w(), PasekWyboruWidoku::WysokoscPrzycisku)
{
	wyswietlanieTekstu.buffer(&buforTekstu);
	wyswietlanieTekstu.hide();

	size_range(800, 600);

	pasekWyboruWidoku.ustaw(&wyswietlanieTekstu, &buforTekstu);

	wyswietlanieTekstu.textfont(FL_COURIER);

	callback(close_callback);

	resizable(grafWidget);
	end();
}
