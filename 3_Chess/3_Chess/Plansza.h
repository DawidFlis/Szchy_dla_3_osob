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
#include <atomic>
#include "MyPhotonListener.h"

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
	sf::Sprite bg, liczba_graczy, tlo_nazwa_pokoju, tlo_nickname, rozgrywka_online, rozgrywka_lokalna;
	std::list<sf::Text> zagrania;
	sf::Text text;
	sf::Texture pr, pw, pp, tabela, g1, g2, res, sw, sw_c, sw_b, lg1, lg2, lg3, oczekiwanie, tnp, tnn, r_online, r_lokalna;
	sf::Texture p[7];
	int w_size, players;
	Pole pola[3][8][4];
	sf::Sprite plansza;
	sf::RenderWindow window;
	sf::Texture tlo;
	sf::Font font;
	bool mozliwe_ruchy[3][8][4], host;
	sf::Texture zp, zp1;
	sf::Sprite z_pole, z_pole1;
	void ustawianie_figur();
	void wczytanie_tekstur();
	void wyswietlanie();
	bool usun_p1 = 0, usun_p2 = 0, menu = 0, wybor_rozgrywki = 1, cala_plansza = 0, przejscie1 = 0, przejscie2 = 0, szach[3] = { 0, 0, 0 };
	std::shared_ptr<Figura> k;
	void przejscia_menu_plansza();
	int i = 0;
	void obsluga_menu(ExitGames::LoadBalancing::Client&);
	sf::Event event;
	bool is_move = 0, poprawne_przeniesienie = 0, online = 0;
	Pole pol_s;
	float dx, dy;
	int ruch = 0;
	sf::Vector2f mouse;
	void przyciski_na_planszy();
	void przeniesienie_figury(ExitGames::LoadBalancing::Client&);
	void przeniesienie_figury_przeciwnika();
	bool spr_szach(std::shared_ptr<Figura>&);
	bool wp_nazw = 0, wp_nazw_pok = 0;
	std::string playerInput, roomname="", nick;
	sf::Text nazwa_b, nazwa_r, nazwa_w, room, nickname;
	sf::Text tczasy[3];
	void wprowadzanie_nazw();
	void wprowadzanie_nazwy_pokoju();
	sf::Texture n;
	sf::Sprite nazwy;
	int aktualna_nazwa=0;
	int czasy[3]={6000,6000,6000};
	sf::Text kolej_g, kolej_gn, szachh;
	sf::Texture cz[4], zat;
	sf::Sprite tryb[4], z_tryb;
	int nr_trybu = 1;
	bool spr_mat(std::shared_ptr<Figura>& krol);
	bool mat = 0, kon_czasu=0, host_started = 0;
	
public:
	void clientServiceLoop(Client& client);
	void glowna_petla();
	void wybieranie_rozgrywki();
};

