#pragma once
#include "Figura.h"
#include <SFML\Graphics.hpp>

class Goniec : public::Figura, public sf::Drawable
{
	void draw(sf::RenderTarget& target, sf::RenderStates state) const override;
	sf::Texture t;
	sf::Sprite s;
	int w_size;
public:



	Goniec(Pole, int);
	sf::Sprite get_sprite();
	void set_position(float, float);
	void set_polozenie(Pole);
	void get_mozliwe_ruchy(bool[3][8][4], std::vector<std::shared_ptr<Figura>>&);
};