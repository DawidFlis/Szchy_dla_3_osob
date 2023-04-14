#include "Krol.h"


Krol::Krol(Pole p, int w)
	:Figura(p, w, "Krol"),
	w_size(w)
{
	if (p.get_czesc_planszy() == 0)
		t.loadFromFile("images/krol_b.png");
	else if (p.get_czesc_planszy() == 1)
		t.loadFromFile("images/krol_c.png");
	else
		t.loadFromFile("images/krol_r.png");
	s.setTexture(t);
	s.setScale(w_size * 0.0004, w_size * 0.0004);
	s.setOrigin(t.getSize().x * 0.5, t.getSize().y / 2);
	s.setPosition(p.get_wx(), p.get_wy());
}

void Krol::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	target.draw(this->s);
}

sf::Sprite Krol::get_sprite()
{
	return s;
}

void Krol::set_position(float wx, float wy)
{
	s.setPosition(wx, wy);
}

void Krol::set_polozenie(Pole p)
{
	s.setPosition(p.get_wx(), p.get_wy());
	set_pole(p);
}