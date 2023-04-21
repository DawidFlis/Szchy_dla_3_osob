#include "Wieza.h"

Wieza::Wieza(Pole p, int w)
	:Figura(p, w, "Wieza"),
	w_size(w)
{
	if (p.get_czesc_planszy() == 0)
		t.loadFromFile("images/wieza_b.png");
	else if (p.get_czesc_planszy() == 1)
		t.loadFromFile("images/wieza_c.png");
	else
		t.loadFromFile("images/wieza_r.png");
	s.setTexture(t);
	s.setScale(w_size * 0.0004, w_size * 0.0004);
	s.setOrigin(t.getSize().x * 0.5, t.getSize().y / 2);
	s.setPosition(p.get_wx(), p.get_wy());
}

void Wieza::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	target.draw(this->s);
}

sf::Sprite Wieza::get_sprite()
{
	return s;
}

void Wieza::set_position(float wx, float wy)
{
	s.setPosition(wx, wy);
}

void Wieza::set_polozenie(Pole p)
{
	s.setPosition(p.get_wx(), p.get_wy());
	set_pole(p);
}

void Wieza::get_mozliwe_ruchy(bool t[3][8][4], std::vector<std::shared_ptr<Figura>>& fig)
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 8; j++)
			for (int k = 0; k < 4; k++)
				t[i][j][k] = 0;

	bool przeciecie = 0;

	for (int i = this->get_pole().get_y()+1; i < 4; i++)
	{
		if (!przeciecie)
		{
			t[this->get_pole().get_czesc_planszy()][this->get_pole().get_x()][i] = 1;
			for (auto j : fig)
				if (this->get_pole().get_czesc_planszy() == j->get_pole().get_czesc_planszy() && this->get_pole().get_x() == j->get_pole().get_x()
					&& j->get_pole().get_y() == i)
				{
					if (this->get_kolor() == j->get_kolor())
						t[this->get_pole().get_czesc_planszy()][this->get_pole().get_x()][i] = 0;
					przeciecie = 1;
				}
		}
	}
	if (!przeciecie)
	{
		if (this->get_pole().get_x() < 4)
		{
			for (int i = 3; i >= 0; i--)
			{
				if (!przeciecie)
				{
					t[(this->get_pole().get_czesc_planszy() + 2) % 3][7 - this->get_pole().get_x()][i] = 1;
					for (auto j : fig)
						if ((this->get_pole().get_czesc_planszy() + 2) % 3 == j->get_pole().get_czesc_planszy() && 7 - this->get_pole().get_x() == j->get_pole().get_x()
							&& j->get_pole().get_y() == i)
						{
							if (this->get_kolor() == j->get_kolor())
								t[(this->get_pole().get_czesc_planszy() + 2) % 3][this->get_pole().get_x()][i] = 0;
							przeciecie = 1;
						}
				}
			}
		}
		else
		{
			for (int i = 3; i >= 0; i--)
			{
				if (!przeciecie)
				{
					t[(this->get_pole().get_czesc_planszy() + 1) % 3][7 - this->get_pole().get_x()][i] = 1;
					for (auto j : fig)
						if ((this->get_pole().get_czesc_planszy() + 1) % 3 == j->get_pole().get_czesc_planszy() && 7 - this->get_pole().get_x() == j->get_pole().get_x()
							&& j->get_pole().get_y() == i)
						{
							if (this->get_kolor() == j->get_kolor())
								t[(this->get_pole().get_czesc_planszy() + 1) % 3][this->get_pole().get_x()][i] = 0;
							przeciecie = 1;
						}
				}
			}
		}
	}
	przeciecie = 0;

	for (int i = this->get_pole().get_y() -1; i>=0; i--)
	{
		if (!przeciecie)
		{
			t[this->get_pole().get_czesc_planszy()][this->get_pole().get_x()][i] = 1;
			for (auto j : fig)
				if (this->get_pole().get_czesc_planszy() == j->get_pole().get_czesc_planszy() && this->get_pole().get_x() == j->get_pole().get_x()
					&& j->get_pole().get_y() == i)
				{
					if (this->get_kolor() == j->get_kolor())
						t[this->get_pole().get_czesc_planszy()][this->get_pole().get_x()][i] = 0;
					przeciecie = 1;
				}
		}
	}
	przeciecie = 0;

	for (int i = this->get_pole().get_x() - 1; i >= 0; i--)
	{
		if (!przeciecie)
		{
			t[this->get_pole().get_czesc_planszy()][i][this->get_pole().get_y()] = 1;
			for (auto j : fig)
				if (this->get_pole().get_czesc_planszy() == j->get_pole().get_czesc_planszy() && i == j->get_pole().get_x()
					&& j->get_pole().get_y() == this->get_pole().get_y())
				{
					if (this->get_kolor() == j->get_kolor())
						t[this->get_pole().get_czesc_planszy()][i][this->get_pole().get_y()] = 0;
					przeciecie = 1;
				}
		}
	}

	przeciecie = 0;

	for (int i = this->get_pole().get_x() + 1; i < 8; i++)
	{
		if (!przeciecie)
		{
			t[this->get_pole().get_czesc_planszy()][i][this->get_pole().get_y()] = 1;
			for (auto j : fig)
				if (this->get_pole().get_czesc_planszy() == j->get_pole().get_czesc_planszy() && i == j->get_pole().get_x()
					&& j->get_pole().get_y() == this->get_pole().get_y())
				{
					if (this->get_kolor() == j->get_kolor())
						t[this->get_pole().get_czesc_planszy()][i][this->get_pole().get_y()] = 0;
					przeciecie = 1;
				}
		}
	}

}
