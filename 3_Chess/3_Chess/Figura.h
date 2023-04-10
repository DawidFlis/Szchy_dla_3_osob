#pragma once
#include "Pole.h"
#include <SFML\Graphics.hpp>

class Figura : public sf::Drawable
{
	//virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const override = 0;
	Pole pol;
	int w_size;
	int kolor;
public:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const override = 0;
	Figura(Pole, int);
	virtual sf::Sprite get_sprite()=0;
	Pole get_pole();
	virtual void set_position(float, float)=0;
	void set_pole(Pole);
	virtual void set_polozenie(Pole)=0;
	int get_kolor();
};

