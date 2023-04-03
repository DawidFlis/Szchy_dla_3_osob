#pragma once
class Pole
{
	int x;
	int y;
	float wx;
	float wy;
	int czesc_planszy;
	int srodek_szachownicy_x;
	int srodek_szachownicy_y;
	float rozmiar_szachownicy;
public:
	Pole(int _x,int _y, int _czesc_planszy, int _srodek_szachownicy_x,int srodek_szachownicy_y, float _rozmiar_szachownicy);
	int get_x();
	int get_y();
	float get_wx();
	float get_wy();

};

