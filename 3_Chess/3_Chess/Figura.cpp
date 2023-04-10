#include "Figura.h"
#include <iostream>


Figura::Figura(Pole p, int w)
	:pol(p),
	w_size(w)
{
	kolor = pol.get_czesc_planszy();
}

Pole Figura::get_pole()
{
	return pol;
}

void Figura::set_pole(Pole p)
{
	pol = p;
}

int Figura::get_kolor()
{
	return kolor;
}
