#pragma once
#include "Pole.h"
#include <SFML\Graphics.hpp>

class Figura:  public sf::Drawable
{

	Pole pol;
	int w_size;
	int kolor;
	std::string nazwa;
public:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const override = 0;
	Figura(Pole, int, std::string);
	virtual sf::Sprite get_sprite()=0;
	Pole get_pole();
	virtual void set_position(float, float)=0;
	void set_pole(Pole);
	virtual void set_polozenie(Pole)=0;
	int get_kolor();
	std::string get_nazwa();
	virtual void get_mozliwe_ruchy(bool [3][8][4], std::vector<std::shared_ptr<Figura>>&)=0;
	virtual void get_mozliwe_bicia(bool[3][8][4], std::vector<std::shared_ptr<Figura>>&);
};

