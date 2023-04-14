#include "Pionek.h"

Pionek::Pionek(Pole p, int w)
	:Figura(p, w, "Pionek"),
	w_size(w)
{
	if (p.get_czesc_planszy() == 0)
		t.loadFromFile("images/pion_b.png");
	else if (p.get_czesc_planszy() == 1)
		t.loadFromFile("images/pion_c.png");
	else
		t.loadFromFile("images/pion_r.png");
	s.setTexture(t);
	s.setScale(w_size * 0.00043, w_size * 0.00043);
	s.setOrigin(t.getSize().x *0.5, t.getSize().y / 2);
	s.setPosition(p.get_wx(), p.get_wy());
}

void Pionek::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	target.draw(this->s);
}

sf::Sprite Pionek::get_sprite()
{
	return s;
}

void Pionek::set_position(float wx, float wy)
{
	s.setPosition(wx, wy);
}

void Pionek::set_polozenie(Pole p)
{
	s.setPosition(p.get_wx(), p.get_wy());
	set_pole(p);
}