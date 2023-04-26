#include "Figura.h"
#include <iostream>


Figura::Figura(Pole p, int w, std::string st)
	:pol(p),
	w_size(w),
	nazwa(st)
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

std::string Figura::get_nazwa()
{
	return nazwa;
}

void Figura::get_mozliwe_bicia(bool t[3][8][4], std::vector<std::shared_ptr<Figura>>&fig)
{
	this->get_mozliwe_ruchy(t, fig);
}