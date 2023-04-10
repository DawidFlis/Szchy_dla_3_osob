#include "Skoczek.h"

Skoczek::Skoczek(Pole p, int w)
	:Figura(p, w),
	w_size(w)
{
	if (p.get_czesc_planszy() == 0)
		t.loadFromFile("images/kon_b.png");
	else if (p.get_czesc_planszy() == 1)
		t.loadFromFile("images/kon_c.png");
	else
		t.loadFromFile("images/kon_r.png");
	s.setTexture(t);
	s.setScale(w_size * 0.00038, w_size * 0.00038);
	s.setOrigin(t.getSize().x * 0.5, t.getSize().y / 2);
	s.setPosition(p.get_wx(), p.get_wy());
}

void Skoczek::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	target.draw(this->s);
}

sf::Sprite Skoczek::get_sprite()
{
	return s;
}

void Skoczek::set_position(float wx, float wy)
{
	s.setPosition(wx, wy);
}

void Skoczek::set_polozenie(Pole p)
{
	s.setPosition(p.get_wx(), p.get_wy());
	set_pole(p);
}