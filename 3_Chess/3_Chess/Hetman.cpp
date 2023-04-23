#include "Hetman.h"


Hetman::Hetman(Pole p, int w)
	:Figura(p, w, "Hetman"),
	w_size(w)
{
	if (p.get_czesc_planszy() == 0)
		t.loadFromFile("images/hetman_b.png");
	else if (p.get_czesc_planszy() == 1)
		t.loadFromFile("images/hetman_c.png");
	else
		t.loadFromFile("images/hetman_r.png");
	s.setTexture(t);
	s.setScale(w_size * 0.0004, w_size * 0.0004);
	s.setOrigin(t.getSize().x * 0.5, t.getSize().y / 2);
	s.setPosition(p.get_wx(), p.get_wy());
}

void Hetman::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	target.draw(this->s);
}

sf::Sprite Hetman::get_sprite()
{
	return s;
}

void Hetman::set_position(float wx, float wy)
{
	s.setPosition(wx, wy);
}

void Hetman::set_polozenie(Pole p)
{
	s.setPosition(p.get_wx(), p.get_wy());
	set_pole(p);
}

void Hetman::get_mozliwe_ruchy(bool t[3][8][4], std::vector<std::shared_ptr<Figura>>& fig)
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 8; j++)
			for (int k = 0; k < 4; k++)
				t[i][j][k] = 0;

	//goniec 

	bool przeciecie = 0;

	{
		int i;
		for (i = 1; i <= std::min(7 - this->get_pole().get_x(), 3 - this->get_pole().get_y()); i++)
		{
			if (!przeciecie)
			{
				t[this->get_pole().get_czesc_planszy()][this->get_pole().get_x() + i][this->get_pole().get_y() + i] = 1;
				for (auto j : fig)
					if (this->get_pole().get_czesc_planszy() == j->get_pole().get_czesc_planszy() && this->get_pole().get_x() + i == j->get_pole().get_x()
						&& j->get_pole().get_y() == this->get_pole().get_y() + i)
					{
						if (this->get_kolor() == j->get_kolor())
							t[this->get_pole().get_czesc_planszy()][this->get_pole().get_x() + i][this->get_pole().get_y() + i] = 0;
						przeciecie = 1;
					}
			}
		}

		if (!przeciecie && this->get_pole().get_x() + i - 1 > 2 && this->get_pole().get_x() + i - 1 < 7)
		{
			for (int k = 0; k <= 3; k++)
			{
				if (!przeciecie && this->get_pole().get_x() + i + k < 8)
				{
					t[(this->get_pole().get_czesc_planszy() + 1) % 3][7 - this->get_pole().get_x() - i - k][3 - k] = 1;

					for (auto j : fig)
						if ((this->get_pole().get_czesc_planszy() + 1) % 3 == j->get_pole().get_czesc_planszy() && 7 - this->get_pole().get_x() - i - k == j->get_pole().get_x()
							&& j->get_pole().get_y() == 3 - k)
						{
							if (this->get_kolor() == j->get_kolor())
								t[(this->get_pole().get_czesc_planszy() + 1) % 3][7 - this->get_pole().get_x() - i - k][3 - k] = 0;
							przeciecie = 1;
						}
				}

			}
			przeciecie = 0;
		}
		if (!przeciecie && this->get_pole().get_x() + i - 1 >= 0 && this->get_pole().get_x() + i - 1 < 4)
		{
			for (int k = 0; k <= 3; k++)
			{
				if (!przeciecie && this->get_pole().get_x() + i + k < 8)
				{
					t[(this->get_pole().get_czesc_planszy() + 2) % 3][7 - this->get_pole().get_x() - i - k][3 - k] = 1;

					for (auto j : fig)
						if ((this->get_pole().get_czesc_planszy() + 2) % 3 == j->get_pole().get_czesc_planszy() && 7 - this->get_pole().get_x() - i - k == j->get_pole().get_x()
							&& j->get_pole().get_y() == 3 - k)
						{
							if (this->get_kolor() == j->get_kolor())
								t[(this->get_pole().get_czesc_planszy() + 2) % 3][7 - this->get_pole().get_x() - i - k][3 - k] = 0;
							przeciecie = 1;
						}

				}
			}
		}
	}

	przeciecie = 0;
	{
		int i;
		for (i = 1; i <= std::min(this->get_pole().get_x(), 3 - this->get_pole().get_y()); i++)
		{
			if (!przeciecie)
			{
				t[this->get_pole().get_czesc_planszy()][this->get_pole().get_x() - i][this->get_pole().get_y() + i] = 1;
				for (auto j : fig)
					if (this->get_pole().get_czesc_planszy() == j->get_pole().get_czesc_planszy() && this->get_pole().get_x() - i == j->get_pole().get_x()
						&& j->get_pole().get_y() == this->get_pole().get_y() + i)
					{
						if (this->get_kolor() == j->get_kolor())
							t[this->get_pole().get_czesc_planszy()][this->get_pole().get_x() - i][this->get_pole().get_y() + i] = 0;
						przeciecie = 1;
					}
			}
		}
		if (!przeciecie && this->get_pole().get_x() - i + 1 > 3 && this->get_pole().get_x() - i + 1 < 8)
		{
			for (int k = 0; k <= 3; k++)
			{
				if (!przeciecie && this->get_pole().get_x() - i - k >= 0)
				{
					t[(this->get_pole().get_czesc_planszy() + 1) % 3][7 - this->get_pole().get_x() + i + k][3 - k] = 1;

					for (auto j : fig)
						if ((this->get_pole().get_czesc_planszy() + 1) % 3 == j->get_pole().get_czesc_planszy() && 7 - this->get_pole().get_x() + i + k == j->get_pole().get_x()
							&& j->get_pole().get_y() == 3 - k)
						{
							if (this->get_kolor() == j->get_kolor())
								t[(this->get_pole().get_czesc_planszy() + 1) % 3][7 - this->get_pole().get_x() + i + k][3 - k] = 0;
							przeciecie = 1;
						}
				}
			}
			przeciecie = 0;
		}
		if (!przeciecie && this->get_pole().get_x() - i + 1 >= 0 && this->get_pole().get_x() - i + 1 < 5)
		{
			for (int k = 0; k <= 3; k++)
			{
				if (!przeciecie && this->get_pole().get_x() - i - k >= 0)
				{
					t[(this->get_pole().get_czesc_planszy() + 2) % 3][7 - this->get_pole().get_x() + i + k][3 - k] = 1;
					for (auto j : fig)
						if ((this->get_pole().get_czesc_planszy() + 2) % 3 == j->get_pole().get_czesc_planszy() && 7 - this->get_pole().get_x() + i + k == j->get_pole().get_x()
							&& j->get_pole().get_y() == 3 - k)
						{
							if (this->get_kolor() == j->get_kolor())
								t[(this->get_pole().get_czesc_planszy() + 2) % 3][7 - this->get_pole().get_x() + i + k][3 - k] = 0;
							przeciecie = 1;
						}
				}
			}
		}

	}

	przeciecie = 0;

	for (int i = 1; i <= std::min(this->get_pole().get_x(), this->get_pole().get_y()); i++)
	{
		if (!przeciecie)
		{
			t[this->get_pole().get_czesc_planszy()][this->get_pole().get_x() - i][this->get_pole().get_y() - i] = 1;
			for (auto j : fig)
				if (this->get_pole().get_czesc_planszy() == j->get_pole().get_czesc_planszy() && this->get_pole().get_x() - i == j->get_pole().get_x()
					&& j->get_pole().get_y() == this->get_pole().get_y() - i)
				{
					if (this->get_kolor() == j->get_kolor())
						t[this->get_pole().get_czesc_planszy()][this->get_pole().get_x() - i][this->get_pole().get_y() - i] = 0;
					przeciecie = 1;
				}
		}
	}

	przeciecie = 0;

	for (int i = 1; i <= std::min(7 - this->get_pole().get_x(), this->get_pole().get_y()); i++)
	{
		if (!przeciecie)
		{
			t[this->get_pole().get_czesc_planszy()][this->get_pole().get_x() + i][this->get_pole().get_y() - i] = 1;
			for (auto j : fig)
				if (this->get_pole().get_czesc_planszy() == j->get_pole().get_czesc_planszy() && this->get_pole().get_x() + i == j->get_pole().get_x()
					&& j->get_pole().get_y() == this->get_pole().get_y() - i)
				{
					if (this->get_kolor() == j->get_kolor())
						t[this->get_pole().get_czesc_planszy()][this->get_pole().get_x() + i][this->get_pole().get_y() - i] = 0;
					przeciecie = 1;
				}
		}
	}

	//wieza

	przeciecie = 0;
	for (int i = this->get_pole().get_y() + 1; i < 4; i++)
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
								t[(this->get_pole().get_czesc_planszy() + 2) % 3][7 - this->get_pole().get_x()][i] = 0;
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
								t[(this->get_pole().get_czesc_planszy() + 1) % 3][7 - this->get_pole().get_x()][i] = 0;
							przeciecie = 1;
						}
				}
			}
		}
	}
	przeciecie = 0;

	for (int i = this->get_pole().get_y() - 1; i >= 0; i--)
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