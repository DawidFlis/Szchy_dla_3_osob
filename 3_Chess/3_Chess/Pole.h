#pragma once
#include <string>

class Pole
{
	int x;
	int y;
	float wx;
	float wy;
	int czesc_planszy;
	int srodek_szachownicy_x;
	int srodek_szachownicy_y;
	int rozmiar_szachownicy;
	std::string pole_tab;
public:
	Pole(int _x,int _y, int _czesc_planszy, int _srodek_szachownicy_x,int srodek_szachownicy_y, float _rozmiar_szachownicy);
	Pole(){};
	int get_x();
	int get_y();
	float get_wx();
	float get_wy();
	int get_czesc_planszy();
	void set_position(float , float);
	std::string get_pole_tab();
	bool operator ==(const Pole& a);
};

