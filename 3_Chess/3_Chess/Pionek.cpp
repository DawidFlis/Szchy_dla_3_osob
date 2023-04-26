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
	s.setScale(w_size * 0.00041, w_size * 0.00041);
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

void Pionek::get_mozliwe_ruchy(bool t[3][8][4], std::vector<std::shared_ptr<Figura>>&fig)
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 8; j++)
			for (int k = 0; k < 4; k++)
				t[i][j][k] = 0;

	if (this->get_kolor() == this->get_pole().get_czesc_planszy())
	{
		if (this->get_pole().get_y() == 1)
		{
			t[this->get_kolor()][this->get_pole().get_x()][this->get_pole().get_y() + 1] = 1;
			t[this->get_kolor()][this->get_pole().get_x()][this->get_pole().get_y() + 2] = 1;
			for (auto i : fig)
				if (i->get_pole().get_czesc_planszy() == this->get_pole().get_czesc_planszy())
				{
					if (i->get_pole().get_x() == this->get_pole().get_x())
					{
						if (i->get_pole().get_y() == this->get_pole().get_y() + 1)
						{
							t[this->get_kolor()][this->get_pole().get_x()][this->get_pole().get_y() + 1] = 0;
							t[this->get_kolor()][this->get_pole().get_x()][this->get_pole().get_y() + 2] = 0;
						}
						else if (i->get_pole().get_y() == this->get_pole().get_y() + 2)
							t[this->get_kolor()][this->get_pole().get_x()][this->get_pole().get_y() + 2] = 0;
					}
					else if(this->get_kolor() != i->get_kolor())
					{
						if (i->get_pole().get_y() == this->get_pole().get_y() + 1)
						{
							if (i->get_pole().get_x() == this->get_pole().get_x() + 1)
								t[this->get_kolor()][this->get_pole().get_x() + 1][this->get_pole().get_y() + 1] = 1;
							else if (i->get_pole().get_x() == this->get_pole().get_x() - 1)
								t[this->get_kolor()][this->get_pole().get_x() - 1][this->get_pole().get_y() + 1] = 1;
						}
					}
				}

		}
		else if (this->get_pole().get_y() == 3)
		{
			if (this->get_pole().get_x() < 4)
			{
				t[(this->get_pole().get_czesc_planszy() + 2) % 3][7 - this->get_pole().get_x()][this->get_pole().get_y()] = 1;
				for (auto i : fig)
					if (i->get_pole().get_y() == 3)
					{
						if (i->get_pole().get_x() == 7 - this->get_pole().get_x() && i->get_pole().get_czesc_planszy() == (this->get_pole().get_czesc_planszy() + 2) % 3)
						{
							t[(this->get_pole().get_czesc_planszy() + 2) % 3][7 - this->get_pole().get_x()][this->get_pole().get_y()] = 0;
						}
						else if (this->get_kolor() != i->get_kolor() && i->get_pole().get_czesc_planszy() == (this->get_pole().get_czesc_planszy() + 2) % 3)
						{
							if (i->get_pole().get_x() == 6 - this->get_pole().get_x())
								t[(this->get_pole().get_czesc_planszy() + 2) % 3][6 - this->get_pole().get_x()][3] = 1;
							else if (i->get_pole().get_x() == 8 - this->get_pole().get_x())
								t[(this->get_pole().get_czesc_planszy() + 2) % 3][8 - this->get_pole().get_x()][3] = 1;
						}
						if (this->get_kolor() != i->get_kolor() && i->get_pole().get_czesc_planszy() == (this->get_pole().get_czesc_planszy() + 1) % 3)
							if (this->get_pole().get_x() == 3 && i->get_pole().get_x() == 3)
								t[(this->get_pole().get_czesc_planszy() + 1) % 3][3][3] = 1;
					}

			}
			else
			{
				t[(this->get_pole().get_czesc_planszy() + 1) % 3][7 - this->get_pole().get_x()][this->get_pole().get_y()] = 1;
				for (auto i : fig)
					if (i->get_pole().get_y() == 3)
					{
						if (i->get_pole().get_x() == 7 - this->get_pole().get_x() && i->get_pole().get_czesc_planszy() == (this->get_pole().get_czesc_planszy() + 1) % 3)
						{
							t[(this->get_pole().get_czesc_planszy() + 1) % 3][7 - this->get_pole().get_x()][this->get_pole().get_y()] = 0;
						}
						else if (this->get_kolor() != i->get_kolor() && i->get_pole().get_czesc_planszy() == (this->get_pole().get_czesc_planszy() + 1) % 3)
						{
							if (i->get_pole().get_x() == 6 - this->get_pole().get_x())
								t[(this->get_pole().get_czesc_planszy() + 1) % 3][6 - this->get_pole().get_x()][3] = 1;
							else if (i->get_pole().get_x() == 8 - this->get_pole().get_x())
								t[(this->get_pole().get_czesc_planszy() + 1) % 3][8 - this->get_pole().get_x()][3] = 1;
						}
						if (this->get_kolor() != i->get_kolor() && i->get_pole().get_czesc_planszy() == (this->get_pole().get_czesc_planszy() + 2) % 3)
							if (this->get_pole().get_x() == 4 && i->get_pole().get_x() == 4)
								t[(this->get_pole().get_czesc_planszy() + 2) % 3][4][3] = 1;
					}

			}
		}
		else
		{
			t[this->get_kolor()][this->get_pole().get_x()][this->get_pole().get_y() + 1] = 1;
			for (auto i : fig)
				if (i->get_pole().get_czesc_planszy() == this->get_pole().get_czesc_planszy() && i->get_pole().get_y() == this->get_pole().get_y() + 1)
					if(i->get_pole().get_x() == this->get_pole().get_x())
						t[this->get_kolor()][this->get_pole().get_x()][this->get_pole().get_y() + 1] = 0;
					else if (this->get_kolor() != i->get_kolor())
					{
						if (i->get_pole().get_x() == this->get_pole().get_x() + 1)
							t[this->get_kolor()][this->get_pole().get_x() + 1][this->get_pole().get_y() + 1] = 1;
						else if (i->get_pole().get_x() == this->get_pole().get_x() - 1)
							t[this->get_kolor()][this->get_pole().get_x() - 1][this->get_pole().get_y() + 1] = 1;
						
					}
		}
	}
	else if(this->get_pole().get_wy()>0)
	{
		t[this->get_pole().get_czesc_planszy()][this->get_pole().get_x()][this->get_pole().get_y() - 1] = 1;
		for (auto i : fig)
			if (i->get_pole().get_czesc_planszy() == this->get_pole().get_czesc_planszy() && i->get_pole().get_y() == this->get_pole().get_y() - 1)
				if(i->get_pole().get_x() == this->get_pole().get_x())
					t[this->get_pole().get_czesc_planszy()][this->get_pole().get_x()][this->get_pole().get_y() - 1] = 0;
				else if (this->get_kolor() != i->get_kolor())
				{
					if (i->get_pole().get_x() == this->get_pole().get_x() + 1)
						t[this->get_pole().get_czesc_planszy()][this->get_pole().get_x() + 1][this->get_pole().get_y() - 1] = 1;
					else if (i->get_pole().get_x() == this->get_pole().get_x() - 1)
						t[this->get_pole().get_czesc_planszy()][this->get_pole().get_x() - 1][this->get_pole().get_y() - 1] = 1;
					
				}
	}

}




void Pionek::get_mozliwe_bicia(bool t[3][8][4], std::vector<std::shared_ptr<Figura>>& fig)
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 8; j++)
			for (int k = 0; k < 4; k++)
				t[i][j][k] = 0;

	if (this->get_kolor() == this->get_pole().get_czesc_planszy())
	{
		if (this->get_pole().get_y() < 3)
		{
			if (8 > this->get_pole().get_x() + 1)
				t[this->get_kolor()][this->get_pole().get_x() + 1][this->get_pole().get_y() + 1] = 1;
			if (0 <= this->get_pole().get_x() - 1)
				t[this->get_kolor()][this->get_pole().get_x() - 1][this->get_pole().get_y() + 1] = 1;
		}
		else if (this->get_pole().get_y() == 3)
		{
			if (this->get_pole().get_x() < 4)
			{

				t[(this->get_pole().get_czesc_planszy() + 2) % 3][6 - this->get_pole().get_x()][3] = 1;
				if (this->get_pole().get_x() != 0)
					t[(this->get_pole().get_czesc_planszy() + 2) % 3][8 - this->get_pole().get_x()][3] = 1;
				if (this->get_pole().get_x() == 3)
					t[(this->get_pole().get_czesc_planszy() + 1) % 3][3][3] = 1;
			}
			else
			{

				if (0 <= 6 - this->get_pole().get_x())
					t[(this->get_pole().get_czesc_planszy() + 1) % 3][6 - this->get_pole().get_x()][3] = 1;
				t[(this->get_pole().get_czesc_planszy() + 1) % 3][8 - this->get_pole().get_x()][3] = 1;
				if (this->get_pole().get_x() == 4)
					t[(this->get_pole().get_czesc_planszy() + 2) % 3][4][3] = 1;

			}
		}
	}
	else if (this->get_pole().get_wy() > 0)
	{
		if (8 > this->get_pole().get_x() + 1)
			t[this->get_pole().get_czesc_planszy()][this->get_pole().get_x() + 1][this->get_pole().get_y() - 1] = 1;
		if (0 <= this->get_pole().get_x() - 1)
			t[this->get_pole().get_czesc_planszy()][this->get_pole().get_x() - 1][this->get_pole().get_y() - 1] = 1;
	}

}