#include "Skoczek.h"

Skoczek::Skoczek(Pole p, int w)
	:Figura(p, w, "Skoczek"),
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

void Skoczek::get_mozliwe_ruchy(bool t[3][8][4], std::vector<std::shared_ptr<Figura>>& fig)
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 8; j++)
			for (int k = 0; k < 4; k++)
				t[i][j][k] = 0;

	for (int i = -2; i < 3; i++)
		for (int j = -2; j < 3; j++)
			if (abs(i) != abs(j) && i!=0 && j!=0)
			{
				if (this->get_pole().get_x() + i < 8 && this->get_pole().get_x() + i >=0 && this->get_pole().get_y() + j >=0)
				{
					if(this->get_pole().get_y() + j < 4)
					{
						t[this->get_pole().get_czesc_planszy()][this->get_pole().get_x() + i][this->get_pole().get_y() + j] = 1;
						for (auto& k : fig)
						{
							if (k->get_kolor() == this->get_kolor() && k->get_pole().get_x() == this->get_pole().get_x() + i 
								&& k->get_pole().get_y() == this->get_pole().get_y() + j&& this->get_pole().get_czesc_planszy()==k->get_pole().get_czesc_planszy())
								t[this->get_pole().get_czesc_planszy()][this->get_pole().get_x() + i][this->get_pole().get_y() + j] = 0;
						}
					}
					else
					{
						if(j==2)
						{
								t[(this->get_pole().get_czesc_planszy() + 2) % 3][7-(this->get_pole().get_x() +i)][5-this->get_pole().get_y() ] = 1;
								t[(this->get_pole().get_czesc_planszy() + 1) % 3][7-(this->get_pole().get_x()+i)][5-this->get_pole().get_y() ] = 1;
								for (auto& k : fig)
								{
									if (k->get_kolor() == this->get_kolor() && k->get_pole().get_x() == 7 - (this->get_pole().get_x() + i) && k->get_pole().get_y() == 5 - this->get_pole().get_y())
									{
										if ((this->get_pole().get_czesc_planszy() + 2) % 3 == k->get_pole().get_czesc_planszy())
											t[(this->get_pole().get_czesc_planszy() + 2) % 3][7 - (this->get_pole().get_x() + i)][5 - this->get_pole().get_y()] = 0;
										if ((this->get_pole().get_czesc_planszy() + 1) % 3 == k->get_pole().get_czesc_planszy())
											t[(this->get_pole().get_czesc_planszy() + 1) % 3][7 - (this->get_pole().get_x() + i)][5 - this->get_pole().get_y()] = 0;
									}
								}
						}
						else
						{
								t[(this->get_pole().get_czesc_planszy() + 2) % 3][7 - (this->get_pole().get_x() + i)][3] = 1;
								t[(this->get_pole().get_czesc_planszy() + 1) % 3][7 - (this->get_pole().get_x() + i)][3] = 1;
								for (auto& k : fig)
								{
									if (k->get_kolor() == this->get_kolor() && k->get_pole().get_x() == 7 - (this->get_pole().get_x() + i) && k->get_pole().get_y() == 3)
									{
										if ((this->get_pole().get_czesc_planszy() + 2) % 3 == k->get_pole().get_czesc_planszy())
											t[(this->get_pole().get_czesc_planszy() + 2) % 3][7 - (this->get_pole().get_x() + i)][3] = 0;
										if ((this->get_pole().get_czesc_planszy() + 1) % 3 == k->get_pole().get_czesc_planszy())
											t[(this->get_pole().get_czesc_planszy() + 1) % 3][7 - (this->get_pole().get_x() + i)][3] = 0;
									}
								}
						}

						if(this->get_pole().get_x()<4)
							for(int i1=4; i1<8;i1++)
								for (int i2 = 0; i2 < 4; i2++)
								{
									t[(this->get_pole().get_czesc_planszy() + 1) % 3][i1][i2] = 0;
								}
						else
							for (int i1 = 0; i1 < 4; i1++)
								for (int i2 = 0; i2 < 4; i2++)
								{
									t[(this->get_pole().get_czesc_planszy() + 2) % 3][i1][i2] = 0;
								}
									


					}
				}

			}
	
}