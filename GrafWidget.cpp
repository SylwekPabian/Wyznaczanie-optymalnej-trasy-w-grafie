#include <FL/Fl.H>
#include <FL/Fl_draw.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Int_Input.H>
#include <string>
#include <cstdio>
#include <array>
#include <cstdlib>
#include <cmath>

#include "GrafWidget.h"
#include "Narzedzia.h"

void okButtonWidgetCallback(Fl_Widget* widget, void*)
{
	GrafWidget* parent = static_cast<GrafWidget*>(widget->parent());
	if (parent->zmienianaDroga != nullptr)
	{
		std::string wartosc(parent->wagaWidget.value());
		if (wartosc.size() <= 5)
		{
			if (!wartosc.empty())
			{
				parent->usatwWageNowejDrogi(std::stoi(wartosc));
			}
		}
		else
		{
			fl_message("Masymalna dozwolona wartosc drogi to 99999");
		}
	}
}

GrafWidget::GrafWidget(int x, int y, int w, int h)
	: Fl_Group(x, y, w, h, nullptr)
	, prawyWcisniety(false)
	, lewyWcisniety(false)
	, wybranyWezel(UINT32_MAX)
	, myszX(0), myszY(0)
	, trybUsuwania(false)
	, wagaWidget(300, 200, 100, 30)
	, okButtonWidget(402, 300, 30, 30, "OK")
	, zmienianaDroga(nullptr)
{
	insert(okButtonWidget, 0);
	okButtonWidget.hide();
	okButtonWidget.callback(okButtonWidgetCallback);
	insert(wagaWidget, 0);
	wagaWidget.hide();
	end();

	wezelOptymalny = fl_rgb_color(100, 234, 117);
	wezelCzerwony = fl_rgb_color(252, 173, 175);
	wezelStart = fl_rgb_color(231, 223, 71);
	wezelKoniec = fl_rgb_color(231, 223, 71);
}

void GrafWidget::draw()
{
	fl_rectf(x(), y(), w(), h(), color()); //czyszczenie kontrolki
	fl_rect(x(), y(), w(), h(), FL_BLACK); //czyszczenie kontrolki

	rysujDrogi();
	rysowanieSzukanieDrogi();
	rysujWezly();

	draw_child(wagaWidget);
	draw_child(okButtonWidget);
}

int GrafWidget::handle(int event)
{
	if (Fl_Group::handle(event)) return 1;

	switch (event)
	{
	case FL_ENTER:
		redraw();
		return 1;

	case FL_LEAVE:
		redraw();
		return 1;

	case FL_PUSH:
		if (zmienianaDroga != nullptr) return 1;
		if (Fl::event_button() == FL_LEFT_MOUSE)
		{
			if (Fl::event_clicks() == 1)
			{
				if (trybUsuwania)
				{
					graf.usunWezel(graf.znajdzWezel(Fl::event_x(), Fl::event_y()));
					graf.usunDroge(Fl::event_x(), Fl::event_y());
				}
				else
				{
					zmienianaDroga = graf.znajdzDroge(Fl::event_x(), Fl::event_y());
					if (graf.znajdzWezel(Fl::event_x(), Fl::event_y()) == UINT32_MAX && zmienianaDroga == nullptr)
					{
						graf.dodajWezel(Fl::event_x(), Fl::event_y());
					}
					else
					{
						pokazPoleEdycjiWagi();
						wagaWidget.value(std::to_string(zmienianaDroga->waga).c_str());
					}
				}
			}
			else
			{
				if (!prawyWcisniety)
				{
					lewyWcisniety = true;
					wybranyWezel = graf.znajdzWezel(Fl::event_x(), Fl::event_y());
				}
			}
			redraw();
		}
		else if (Fl::event_button() == FL_RIGHT_MOUSE)
		{
			if (!lewyWcisniety)
			{
				prawyWcisniety = true;
				wybranyWezel = graf.znajdzWezel(Fl::event_x(), Fl::event_y());
			}
		}
		return 1;

	case FL_RELEASE:
		if (zmienianaDroga != nullptr) return 1;
		if (Fl::event_button() == FL_RIGHT_MOUSE)
		{
			prawyWcisniety = false;
			zmienianaDroga = graf.dodajDroge(wybranyWezel, graf.znajdzWezel(Fl::event_x(), Fl::event_y()), 1);
			pokazPoleEdycjiWagi();
		}
		else if (Fl::event_button() == FL_LEFT_MOUSE)
		{
			lewyWcisniety = false;
			wybranyWezel = UINT32_MAX;
		}
		redraw();
		return 1;

	case FL_DRAG:
		myszX = Fl::event_x();
		myszY = Fl::event_y();
		if (lewyWcisniety && wybranyWezel != UINT32_MAX)
		{
			graf.wezly[wybranyWezel].x = myszX;
			graf.wezly[wybranyWezel].y = myszY;
		}
		redraw();
		return 1;
	}
	
	return 0;
}

void GrafWidget::rysowanieSzukanieDrogi()
{
	if (prawyWcisniety && wybranyWezel != UINT32_MAX)
	{
		fl_color(FL_BLACK);
		fl_line(graf.wezly[wybranyWezel].x , graf.wezly[wybranyWezel].y, myszX, myszY);
	}
}

void GrafWidget::rysujDrogi()
{
	for (auto & wezel : graf.wezly)
	{
		for (uint32_t i = 0; i < wezel.drogi.size(); i++)
		{
			if (wezel.drogi[i].waga != INT32_MAX)
			{
				fl_color(FL_BLACK);

				int32_t wx = static_cast<int32_t>(wezel.x) - static_cast<int32_t>(graf.wezly[i].x);
				int32_t wy = static_cast<int32_t>(wezel.y) - static_cast<int32_t>(graf.wezly[i].y);
				double dlugW = sqrt(wx * wx + wy * wy);
				double wxn = wx / dlugW;
				double wyn = wy / dlugW;
				int32_t strzalkaX = static_cast<uint32_t>((wxn * Graf::rozmiarWezla) + graf.wezly[i].x);
				int32_t strzalkaY = static_cast<uint32_t>((wyn * Graf::rozmiarWezla) + graf.wezly[i].y);

				rysujStrzalke(wezel.x, wezel.y, strzalkaX, strzalkaY);

				wezel.drogi[i].x = (wezel.x + graf.wezly[i].x) / 2;
				wezel.drogi[i].y = (wezel.y + graf.wezly[i].y) / 2;

				Fl_Color kolorDrogi;
				if (wezel.drogi[i].zaznaczenie == Graf::OPTYMALNE) kolorDrogi = wezelOptymalny;
				else if (wezel.drogi[i].zaznaczenie == Graf::PODSWIETL_CZERWONY) kolorDrogi = wezelCzerwony;
				else kolorDrogi = color();

				fl_rectf(wezel.drogi[i].x - (wezel.drogi[i].szerokosc / 2),
					wezel.drogi[i].y - (wezel.drogi[i].wysokosc / 2),
					wezel.drogi[i].szerokosc, wezel.drogi[i].wysokosc, kolorDrogi);
				fl_rect(wezel.drogi[i].x - (wezel.drogi[i].szerokosc / 2),
					wezel.drogi[i].y - (wezel.drogi[i].wysokosc / 2),
					wezel.drogi[i].szerokosc, wezel.drogi[i].wysokosc, FL_BLACK);

				pisz(std::to_string(wezel.drogi[i].waga), wezel.drogi[i].x, wezel.drogi[i].y, Graf::rozmiarCzcionkiDrogi);
			}
		}
	}
}

void GrafWidget::rysujWezly()
{
	for (uint32_t i = 0; i < graf.wezly.size(); i++)
	{
		fl_color(fl_rgb_color(0, 162, 232));
		if (graf.wezly[i].zaznaczenie == Graf::OPTYMALNE) fl_color(wezelOptymalny);
		else if (graf.wezly[i].zaznaczenie == Graf::PODSWIETL_CZERWONY) fl_color(wezelCzerwony);

		if (i == 0) fl_color(wezelStart);
		else if (i == graf.wezly.size() - 1) fl_color(wezelKoniec);

		fl_pie(graf.wezly[i].x - Graf::rozmiarWezla, graf.wezly[i].y - Graf::rozmiarWezla,
			2 * Graf::rozmiarWezla, 2 * Graf::rozmiarWezla, 0, 360);

		fl_color(FL_BLACK);
		fl_arc(graf.wezly[i].x - Graf::rozmiarWezla, graf.wezly[i].y - Graf::rozmiarWezla,
			2 * Graf::rozmiarWezla, 2 * Graf::rozmiarWezla, 0, 360);

		pisz(std::to_string(i + 1), graf.wezly[i].x, graf.wezly[i].y, Graf::rozmiarWezla);
	}
}

void GrafWidget::pisz(std::string tekst, uint32_t x, uint32_t y, uint32_t rozmiar)
{
	fl_font(FL_HELVETICA_BOLD, rozmiar);
	int descent = fl_descent();
	std::array<int, 2> tab = Narzedzia::rozmiarTekstu(tekst, rozmiar);

	int tx = x - (tab[0] / 2);
	int ty = y - (tab[1] / 2) + tab[1] - descent;
	fl_color(FL_BLACK);
	fl_draw(tekst.c_str(), tx, ty);
}

int Round(float x) { return static_cast<int>(x + 0.5f); }
void GrafWidget::rysujStrzalke(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1)
{
	fl_color(FL_BLACK);
	fl_line(x0, y0, x1, y1);

	const uint32_t dlugoscGrotu = 8;
	const uint32_t szerokoscGrotu = 5;

	const float dx = static_cast<float>(x1) - static_cast<float>(x0);
	const float dy = static_cast<float>(y1) - static_cast<float>(y0);
	const auto dlugosc = sqrt(dx * dx + dy * dy);

	if (dlugosc < dlugoscGrotu) return;

	const auto ux = dx / dlugosc;
	const auto uy = dy / dlugosc;
	const auto vx = -uy;
	const auto vy = ux;
	const auto srodekStrzalki = 0.5f * dlugoscGrotu;

	const int32_t hx1 = Round(x1 - dlugoscGrotu * ux + srodekStrzalki * vx);
	const int32_t hy1 = Round(y1 - dlugoscGrotu * uy + srodekStrzalki * vy);
	const int32_t hx2 = Round(x1 - dlugoscGrotu * ux - srodekStrzalki * vx);
	const int32_t hy2 = Round(y1 - dlugoscGrotu * uy - srodekStrzalki * vy);

	fl_polygon(x1, y1, hx1, hy1, hx2, hy2);
}

void GrafWidget::pokazPoleEdycjiWagi()
{
	if (zmienianaDroga)
	{
		okButtonWidget.show();
		wagaWidget.show();
		wagaWidget.value("1");

		wagaWidget.resize(zmienianaDroga->x - ((wagaWidget.w() + okButtonWidget.w() + 2) / 2),
			zmienianaDroga->y - (wagaWidget.h() / 2), wagaWidget.w(), wagaWidget.h());
		okButtonWidget.position((wagaWidget.x() + 2 + wagaWidget.w()),
			zmienianaDroga->y - (wagaWidget.h() / 2));
	}
}

void GrafWidget::zmienTrybEdycji()
{
	trybUsuwania = !trybUsuwania;
}

void GrafWidget::zapiszDoPliku()
{
	Fl_File_Chooser wyborPliku(".", "*.graf", Fl_File_Chooser::CREATE, "Zapisz graf");
	wyborPliku.show();

	while (wyborPliku.shown()) { Fl::wait();  }

	if (wyborPliku.value() != nullptr)
	{
		FILE *file = fopen(wyborPliku.value(), "w");
		fprintf(file, "grafmodel1\n");
		fprintf(file, "%d\n", graf.wezly.size());
		for (const auto &wezel : graf.wezly)
		{
			fprintf(file, "%d %d %d\n", wezel.x, wezel.y, wezel.drogi.size());
			for (const auto &droga : wezel.drogi)
			{
				fprintf(file, "%d %d %d %d %d ", droga.x, droga.y, droga.waga, droga.szerokosc, droga.wysokosc);
			}
			fprintf(file, "\n");
		}
		fclose(file);
	}
}

void GrafWidget::wczytajZPliku()
{
	Fl_File_Chooser wyborPliku(".", "*.graf", Fl_File_Chooser::SINGLE, "Wczytaj graf");
	wyborPliku.show();

	while (wyborPliku.shown()) { Fl::wait(); }

	if (wyborPliku.value() != nullptr)
	{
		graf.wezly.clear();
		FILE *file = fopen(wyborPliku.value(), "r");
		char typ[15];
		std::string grafTyp = "grafmodel1";
		fscanf(file, "%s", &typ);

		if (grafTyp == typ)
		{
			uint32_t liczbaWezlow;
			fscanf(file, "%d", &liczbaWezlow);

			for (uint32_t i = 0; i < liczbaWezlow; i++)
			{
				Graf::Wezel wezel;
				uint32_t liczbaDrog;
				fscanf(file, "%d %d %d", &wezel.x, &wezel.y, &liczbaDrog);
				for (uint32_t j = 0; j < liczbaDrog; j++)
				{
					Graf::Droga droga;
					fscanf(file, "%d %d %d %d %d", &droga.x, &droga.y, &droga.waga, &droga.szerokosc, &droga.wysokosc);
					wezel.drogi.push_back(droga);
				}
				graf.wezly.push_back(wezel);
			}
		}
		else
		{
			fl_message("Zly format pliku!");
		}
	}
}

void GrafWidget::usatwWageNowejDrogi(int32_t waga)
{
	graf.zmienWageDrogi(*zmienianaDroga, waga);
	zmienianaDroga = nullptr;
	wagaWidget.hide();
	okButtonWidget.hide();
}
