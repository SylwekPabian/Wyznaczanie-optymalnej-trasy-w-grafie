#include <FL\Fl.H>

#include "Okno.h"

int main(int argc, char **argv)
{
	Fl::scheme("gtk+");
	Okno okno(800, 600, "Badanie operacyjne - Wyznaczanie optymalnej trasy w grafie");
	
	okno.end();
	okno.show(argc, argv);

	return Fl::run();
}