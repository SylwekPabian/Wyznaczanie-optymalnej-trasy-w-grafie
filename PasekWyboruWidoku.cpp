#include "PasekWyboruWidoku.h"

#include <string>

const uint32_t PasekWyboruWidoku::WysokoscPrzycisku = 25;
const uint32_t PasekWyboruWidoku::SzerokoscPrzycisku = 140;

void ZmienWidokCallback(Fl_Widget* widget, void*)
{
	PasekWyboruWidoku *pasekWyboru = static_cast<PasekWyboruWidoku *>(widget->parent());
	if ("Graf" == widget->label())
	{
		pasekWyboru->wybranyWidok = PasekWyboruWidoku::WIDOK_GRAFU;
		pasekWyboru->grafWidget->show();
		pasekWyboru->wyswietlanieTekstu->hide();
		pasekWyboru->buforTekstu->text("");
	}
	else if ("Wynik algorytmu" == widget->label())
	{
		pasekWyboru->wybranyWidok = PasekWyboruWidoku::WIDOK_WYNIK;
		pasekWyboru->grafWidget->hide();
		pasekWyboru->wyswietlanieTekstu->show();
		pasekWyboru->buforTekstu->text(pasekWyboru->grafWidget->graf.pobierzWynikAlgorytmu().c_str());
	}
	else if ("Macierz asocjacyjna" == widget->label())
	{
		pasekWyboru->wybranyWidok = PasekWyboruWidoku::WIDOK_ASOC;
		pasekWyboru->grafWidget->hide();
		pasekWyboru->wyswietlanieTekstu->show();
		pasekWyboru->buforTekstu->text(pasekWyboru->grafWidget->graf.wygenerujMacierzAsoc().c_str());
	}
	else if ("Macierz kosztów" == widget->label())
	{
		pasekWyboru->wybranyWidok = PasekWyboruWidoku::WIDOK_KOSZ;
		pasekWyboru->grafWidget->hide();
		pasekWyboru->buforTekstu->text(pasekWyboru->grafWidget->graf.wygenerujMacierzKosz().c_str());
		pasekWyboru->wyswietlanieTekstu->show();

		FILE* f = fopen("test.txt", "w");
		fprintf(f, "%s", pasekWyboru->grafWidget->graf.wygenerujMacierzKosz().c_str());
		fclose(f);
	}
	else if ("Autorzy" == widget->label())
	{
		pasekWyboru->wybranyWidok = PasekWyboruWidoku::WIDOK_AUTORZY;
		pasekWyboru->grafWidget->hide();
		pasekWyboru->wyswietlanieTekstu->show();
		std::string autorzy = "\nAutorzy:\n Mrzyglod Szczepan (K), \n Pabian Sylwester, \n Polek Leszek, \n Ciochon Przemyslaw, \n Kolodziej Krzysztof, \n Budzik Krystian, \n Piekosz Milosz, \n Reczek Kamil\n\nOpiekun projektu:\n prof. dr hab. in¿. Boguslaw Filipowicz";
		pasekWyboru->buforTekstu->text(autorzy.c_str());
	}
}

PasekWyboruWidoku::PasekWyboruWidoku(GrafWidget * grafWidget_, int x, int y, int szerokosc, int wysokosc)
	: Fl_Pack(x, y, szerokosc, wysokosc)
	, grafWidget(grafWidget_)
	, widokLabel(0, 0, 60, WysokoscPrzycisku, "Widok")
	, widokGrafu(0, 0, SzerokoscPrzycisku, WysokoscPrzycisku, "Graf")
	, widokWynikAlgorytmu(0, 0, SzerokoscPrzycisku, WysokoscPrzycisku, "Wynik algorytmu")
	, widokMacierzAsoc(0, 0, SzerokoscPrzycisku, WysokoscPrzycisku, "Macierz asocjacyjna")
	, widokMacierzKosz(0, 0, SzerokoscPrzycisku, WysokoscPrzycisku, "Macierz kosztów")
	, widokAutorzy(0, 0, SzerokoscPrzycisku, WysokoscPrzycisku, "Autorzy")
{
	type(Fl_Pack::HORIZONTAL);
	spacing(4);

	widokLabel.labelsize(12);

	konfigurujPrzyciskRadio(widokGrafu);
	konfigurujPrzyciskRadio(widokWynikAlgorytmu);
	konfigurujPrzyciskRadio(widokMacierzAsoc);
	konfigurujPrzyciskRadio(widokMacierzKosz);
	konfigurujPrzyciskRadio(widokAutorzy);

	widokGrafu.value(1);
	wybranyWidok = PasekWyboruWidoku::WIDOK_GRAFU;

	end();
}

void PasekWyboruWidoku::ustaw(Fl_Text_Display * wyswietlanieTekstu, Fl_Text_Buffer * buforTekstu)
{
	this->wyswietlanieTekstu = wyswietlanieTekstu;
	this->buforTekstu = buforTekstu;
}

void PasekWyboruWidoku::konfigurujPrzyciskRadio(Fl_Button & przycisk)
{
	przycisk.labelsize(12);
	przycisk.color(przycisk.color(), FL_GREEN);
	przycisk.type(FL_RADIO_BUTTON);
	przycisk.callback(ZmienWidokCallback);
}
