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

void Krol::get_mozliwe_ruchy(bool t[3][8][4], std::vector<std::shared_ptr<Figura>>& fig)
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 8; j++)
			for (int k = 0; k < 4; k++)
				t[i][j][k] = 0;

	for (int i = -1; i < 2; i++)
		for (int j = -1; j < 2; j++)
		{
			if (this->get_pole().get_x() + i >= 0 && this->get_pole().get_x() + i < 8 && this->get_pole().get_y() + j >= 0 && !(i == 0 && j == 0))
			{
				if (this->get_pole().get_y() + j < 4)
				{
					t[this->get_pole().get_czesc_planszy()][this->get_pole().get_x() + i][this->get_pole().get_y() + j] = 1;
					for (auto k : fig)
						if (this->get_pole().get_czesc_planszy() == k->get_pole().get_czesc_planszy() && this->get_pole().get_x() + i == k->get_pole().get_x()
							&& k->get_pole().get_y() == this->get_pole().get_y() + j && this->get_kolor() == k->get_kolor())
								t[this->get_pole().get_czesc_planszy()][this->get_pole().get_x() + i][this->get_pole().get_y() +j] = 0;
						
				}
				else
				{
					if (this->get_pole().get_x()  < 4)
					{
						t[(this->get_pole().get_czesc_planszy()+2)%3][7-this->get_pole().get_x() - i][3] = 1;
						for (auto k : fig)
							if ((this->get_pole().get_czesc_planszy() + 2) % 3 == k->get_pole().get_czesc_planszy() && 7 - this->get_pole().get_x() - i == k->get_pole().get_x()
								&& k->get_pole().get_y() == 3 && this->get_kolor() == k->get_kolor())
								t[(this->get_pole().get_czesc_planszy() + 2) % 3][7-this->get_pole().get_x() - i][3] = 0;
					}
					else
					{
						t[(this->get_pole().get_czesc_planszy() + 1) % 3][7-this->get_pole().get_x() - i][3] = 1;
						for (auto k : fig)
							if ((this->get_pole().get_czesc_planszy() + 1)%3 == k->get_pole().get_czesc_planszy() && 7 - this->get_pole().get_x() - i == k->get_pole().get_x()
								&& k->get_pole().get_y() == 3 && this->get_kolor() == k->get_kolor())
								t[(this->get_pole().get_czesc_planszy() + 1)%3][7 - this->get_pole().get_x() - i][3] = 0;
					}
					if (this->get_pole().get_x() == 3)
					{
						t[(this->get_pole().get_czesc_planszy() + 1) % 3][3][3] = 1;
						for (auto k : fig)
							if ((this->get_pole().get_czesc_planszy() + 1) % 3 == k->get_pole().get_czesc_planszy() && 3 == k->get_pole().get_x()
								&& k->get_pole().get_y() == 3 && this->get_kolor() == k->get_kolor())
								t[(this->get_pole().get_czesc_planszy() + 1) % 3][3][3] = 0;
					}
					else if (this->get_pole().get_x() == 4)
					{
						t[(this->get_pole().get_czesc_planszy() + 2) % 3][4][3] = 1;
						for (auto k : fig)
							if ((this->get_pole().get_czesc_planszy() + 1) % 4 == k->get_pole().get_czesc_planszy() && 3 == k->get_pole().get_x()
								&& k->get_pole().get_y() == 3 && this->get_kolor() == k->get_kolor())
								t[(this->get_pole().get_czesc_planszy() + 1) % 3][4][3] = 0;
					}
				}
			}
		}
}