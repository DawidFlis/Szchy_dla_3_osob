#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Pole.h"
#include <vector>
#include "Figura.h"
#include "Pionek.h"
#include "Wieza.h"
#include "Goniec.h"
#include "Skoczek.h"
#include "Krol.h"
#include "Hetman.h"
#include <list>
#include <regex>
#include <chrono>
#include <thread>

class Plansza
{
	std::vector<std::shared_ptr<Figura>> fig; 
	sf::Sprite rozpocznij;  
	sf::Sprite wyjdz; 
	sf::Sprite powrot;
	sf::Sprite tab;
	sf::Sprite glosnik; 
	sf::Sprite restart; 
	sf::Sprite podswietlenie;
	sf::Sprite bg;
	std::list<sf::Text> zagrania;
	sf::Text text;
	sf::Texture pr, pw, pp, tabela, g1, g2, res, sw, sw_c, sw_b;
	sf::Texture p[7];
	int w_size;
	Pole pola[3][8][4];
	sf::Sprite plansza;
	sf::RenderWindow window;
	sf::Texture tlo;
	sf::Font font;
	bool mozliwe_ruchy[3][8][4];
	sf::Texture zp, zp1;
	sf::Sprite z_pole, z_pole1;
	void ustawianie_figur();
	void wczytanie_tekstur();
	void wyswietlanie();
	bool usun_p1 = 0, usun_p2 = 0, menu = 1, cala_plansza = 0, przejscie1 = 0, przejscie2 = 0, szach[3] = { 0, 0, 0 };
	std::shared_ptr<Figura> k;
	void przejscia_menu_plansza();
	int i = 0;
	void obsluga_menu();
	sf::Event event;
	bool is_move = 0, poprawne_przeniesienie = 0;
	Pole pol_s;
	float dx, dy;
	int ruch = 0;
	sf::Vector2f mouse;
	void przyciski_na_planszy();
	void przeniesienie_figury();
	bool spr_szach(std::shared_ptr<Figura>&);
	bool wp_nazw = 1;
	std::string playerInput;
	sf::Text nazwa_b, nazwa_r, nazwa_w;
	sf::Text tczasy[3];
	void wprowadzanie_nazw();
	sf::Texture n;
	sf::Sprite nazwy;
	int aktualna_nazwa=0;
	int czasy[3]={6000,6000,6000};
	sf::Text kolej_g, kolej_gn, szachh;
	sf::Texture cz[4], zat;
	sf::Sprite tryb[4], z_tryb;
	int nr_trybu = 1;
	bool spr_mat(std::shared_ptr<Figura>& krol);
	bool mat = 0, kon_czasu=0;
	
public:
	void glowna_petla();
};

