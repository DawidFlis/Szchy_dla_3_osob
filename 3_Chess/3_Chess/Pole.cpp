#include "Pole.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <cstdlib>

#define M_PI 3.14159265358979323846

Pole::Pole(int _x, int _y, int _czesc_planszy, int _srodek_szachownicy_x, int _srodek_szachownicy_y, float _rozmiar_szachownicy)
	:x(_x),
	y(_y),
	czesc_planszy(_czesc_planszy),
	srodek_szachownicy_x(_srodek_szachownicy_x),
	srodek_szachownicy_y(_srodek_szachownicy_y),
	rozmiar_szachownicy(_rozmiar_szachownicy)
{
		if (x>3)
		{
			wy = srodek_szachownicy_y-0.105*rozmiar_szachownicy*(y+1) + 0.47 * rozmiar_szachownicy + 0.013 * (x-3.5 ) * (y +0.5) * rozmiar_szachownicy;  //NIE RUSZAC
			wx = srodek_szachownicy_x +(x-3)*rozmiar_szachownicy*0.06-0.03 * rozmiar_szachownicy+ 0.0075 * (x - 3.5) * (y + 0.5) * rozmiar_szachownicy;

		}
		else
		{
			wy = srodek_szachownicy_y - 0.105 * rozmiar_szachownicy * (y + 1) + 0.47 * rozmiar_szachownicy + 0.013 * (x +0.5) * (y + 0.5) * rozmiar_szachownicy;
			wx = srodek_szachownicy_x - (x +1) * rozmiar_szachownicy * 0.06 + 0.03 * rozmiar_szachownicy - 0.0075 * (x +0.5) * (y + 0.5) * rozmiar_szachownicy;
		}
		if (czesc_planszy == 1)
		{
			float pwx = wx;
			wx = (wx-srodek_szachownicy_x) * cos(M_PI *0.66666) - (srodek_szachownicy_y-wy) * sin(M_PI * 0.66666)+srodek_szachownicy_x+ 0.01 * rozmiar_szachownicy;
			wy = srodek_szachownicy_y-(pwx - srodek_szachownicy_x) * sin(M_PI * 0.66666) - (srodek_szachownicy_y - wy) * cos(M_PI * 0.66666)- 0.01 * rozmiar_szachownicy;
		}
		else if (czesc_planszy == 2)
		{
			float pwx = wx;
			wx = (wx - srodek_szachownicy_x) * cos(M_PI * 1.333333) - (srodek_szachownicy_y - wy) * sin(M_PI * 1.333333) + srodek_szachownicy_x - 0.01 * rozmiar_szachownicy;
			wy = srodek_szachownicy_y - (pwx - srodek_szachownicy_x) * sin(M_PI * 1.333333) - (srodek_szachownicy_y - wy) * cos(M_PI * 1.333333) - 0.01 * rozmiar_szachownicy;
		}
}

int Pole::get_x()
{
	return x;
}
int Pole::get_y()
{
	return y;
}
float Pole::get_wx()
{
	return wx;
}
float Pole::get_wy()
{
	return wy;
}