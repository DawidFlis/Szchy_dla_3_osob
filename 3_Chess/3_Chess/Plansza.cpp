#include "Plansza.h"


void pola_w_tablicy(Pole pola[3][8][4], int sx, int sy, int rozmiar);


void Plansza::glowna_petla()
{
    window.create(sf::VideoMode(16000, 9000), "Szachy dla 3 osób", sf::Style::Fullscreen);
    window.setFramerateLimit(50);
    wczytanie_tekstur();
    pola_w_tablicy(pola, window.getSize().x / 2, window.getSize().y / 2, w_size);
    ustawianie_figur();

    k = fig[0];

    while (window.isOpen())
    {

        mouse = (sf::Vector2f)sf::Mouse::getPosition(window);

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (menu == 1)
                obsluga_menu();
            if (cala_plansza)
                przyciski_na_planszy();

            if (event.type == sf::Event::MouseButtonPressed)
                if (event.key.code == sf::Mouse::Left)
                    for (auto i : fig)
                        if (i->get_sprite().getGlobalBounds().contains(mouse) && i->get_kolor() == ruch)
                        {
                            pol_s = i->get_pole();
                            k = i;
                            k->get_mozliwe_ruchy(mozliwe_ruchy, fig);
                            is_move = 1;
                            dx = k->get_pole().get_wx() - mouse.x;
                            dy = k->get_pole().get_wy() - mouse.y;
                        }

            if (event.type == sf::Event::MouseButtonReleased && is_move)
                if (event.key.code == sf::Mouse::Left)
                    przeniesienie_figury();
        }

        if (is_move)
            k->set_position(mouse.x + dx, mouse.y + dy);

        przejscia_menu_plansza();
        wyswietlanie();
    }

}





void Plansza::ustawianie_figur()
{

    for (int ii = 0; ii < 8; ii++)
    {
        std::shared_ptr<Figura> pionekb = std::make_shared<Pionek>(pola[0][ii][1], w_size);
        fig.push_back(pionekb);
        std::shared_ptr<Figura> pionekc = std::make_shared<Pionek>(pola[1][ii][1], w_size);
        fig.push_back(pionekc);
        std::shared_ptr<Figura> pionekr = std::make_shared<Pionek>(pola[2][ii][1], w_size);
        fig.push_back(pionekr);
    }
    std::shared_ptr<Figura> wieza = std::make_shared<Wieza>(pola[0][0][0], w_size);
    fig.push_back(wieza);
    wieza = std::make_shared<Wieza>(pola[1][0][0], w_size);
    fig.push_back(wieza);
    wieza = std::make_shared<Wieza>(pola[2][0][0], w_size);
    fig.push_back(wieza);
    wieza = std::make_shared<Wieza>(pola[0][7][0], w_size);
    fig.push_back(wieza);
    wieza = std::make_shared<Wieza>(pola[1][7][0], w_size);
    fig.push_back(wieza);
    wieza = std::make_shared<Wieza>(pola[2][7][0], w_size);
    fig.push_back(wieza);

    std::shared_ptr<Figura> goniec = std::make_shared<Goniec>(pola[0][2][0], w_size);
    fig.push_back(goniec);
    goniec = std::make_shared<Goniec>(pola[1][2][0], w_size);
    fig.push_back(goniec);
    goniec = std::make_shared<Goniec>(pola[2][2][0], w_size);
    fig.push_back(goniec);
    goniec = std::make_shared<Goniec>(pola[0][5][0], w_size);
    fig.push_back(goniec);
    goniec = std::make_shared<Goniec>(pola[1][5][0], w_size);
    fig.push_back(goniec);
    goniec = std::make_shared<Goniec>(pola[2][5][0], w_size);
    fig.push_back(goniec);

    for (int j = 0; j < 3; j++)
    {
        std::shared_ptr<Figura> skoczek = std::make_shared<Skoczek>(pola[j][1][0], w_size);
        fig.push_back(skoczek);
        skoczek = std::make_shared<Skoczek>(pola[j][6][0], w_size);
        fig.push_back(skoczek);
    }

    std::shared_ptr<Figura> krol = std::make_shared<Krol>(pola[0][4][0], w_size);
    fig.push_back(krol);
    krol = std::make_shared<Krol>(pola[1][4][0], w_size);
    fig.push_back(krol);
    krol = std::make_shared<Krol>(pola[2][4][0], w_size);
    fig.push_back(krol);

    std::shared_ptr<Figura> hetman = std::make_shared<Hetman>(pola[0][3][0], w_size);
    fig.push_back(hetman);
    hetman = std::make_shared<Hetman>(pola[1][3][0], w_size);
    fig.push_back(hetman);
    hetman = std::make_shared<Hetman>(pola[2][3][0], w_size);
    fig.push_back(hetman);

}

void Plansza::wczytanie_tekstur()
{
    
    tlo.loadFromFile("images/tlo.png");
    bg.setTexture(tlo);
    bg.setScale((float)window.getSize().x / tlo.getSize().x, (float)window.getSize().y / tlo.getSize().y);
    w_size = window.getSize().y;

    
    p[0].loadFromFile("images/plansza.png");
    p[1].loadFromFile("images/plansza2.png");
    p[2].loadFromFile("images/plansza3.png");
    p[3].loadFromFile("images/plansza4.png");
    p[4].loadFromFile("images/plansza5.png");
    p[5].loadFromFile("images/plansza6.png");
    p[6].loadFromFile("images/plansza7.png");
    plansza.setTexture(p[1]);


    pr.loadFromFile("images/rozpocznij2.png");
    pw.loadFromFile("images/wyjdz.png");
    pp.loadFromFile("images/powrot1.png");
    tabela.loadFromFile("images/tabela.png");
    g1.loadFromFile("images/sound_on.png");
    g2.loadFromFile("images/sound_off.png");
    res.loadFromFile("images/restart.png");
    sw.loadFromFile("images/sw.png");
    sw_c.loadFromFile("images/sw_c.png");
    sw_b.loadFromFile("images/sw_b.png");
    zp.loadFromFile("images/z_pole2.png");
    zp1.loadFromFile("images/z_pole1.png");

    rozpocznij.setTexture(pr);
    wyjdz.setTexture(pw);
    powrot.setTexture(pp);
    tab.setTexture(tabela);
    glosnik.setTexture(g1);
    restart.setTexture(res);
    podswietlenie.setTexture(sw_b);

    z_pole.setTexture(zp);
    z_pole.setOrigin(zp.getSize().x / 2, zp.getSize().y / 2);
    z_pole.setScale(w_size * 0.0008, w_size * 0.0008);
    z_pole1.setTexture(zp1);
    z_pole1.setOrigin(zp.getSize().x / 2, zp.getSize().y / 2);
    z_pole1.setScale(w_size * 0.0007, w_size * 0.0007);
 
   

    tab.setScale(window.getSize().y * 0.0003, window.getSize().y * 0.0003);
    tab.setOrigin(tabela.getSize().x, 0);
    tab.setPosition(window.getSize().x * 0.99, window.getSize().y * 0.01);

    glosnik.setScale(window.getSize().y * 0.00018, window.getSize().y * 0.00017);
    glosnik.setPosition(window.getSize().x * 0.13, window.getSize().y * 0.06);
    glosnik.setOrigin(g1.getSize().x * 0.5, g1.getSize().y * 0.5);

    restart.setScale(window.getSize().y * 0.00018, window.getSize().y * 0.00017);
    restart.setPosition(window.getSize().x * 0.08, window.getSize().y * 0.06);
    restart.setOrigin(res.getSize().x * 0.5, res.getSize().y * 0.5);

    powrot.setScale(window.getSize().y * 0.00018, window.getSize().y * 0.00017);
    powrot.setPosition(window.getSize().x * 0.03, window.getSize().y * 0.06);
    powrot.setOrigin(pp.getSize().x * 0.5, pp.getSize().y * 0.5);

    podswietlenie.setOrigin(sw.getSize().x / 2, sw.getSize().y / 2);
    podswietlenie.setPosition(window.getSize().x * 0.5, window.getSize().y * 0.5);
    podswietlenie.setScale(window.getSize().y * 0.0005, window.getSize().y * 0.0007);


    font.loadFromFile("ARIBL0.ttf");
    text.setFont(font);
    std::list<sf::Text> zagrania;
    text.setScale(w_size * 0.0005, w_size * 0.0005);
    text.setPosition(window.getSize().x * 0.915, window.getSize().y * 0.07);

    //pola_w_tablicy(pola, window.getSize().x / 2, window.getSize().y / 2, window.getSize().y);


    plansza.setOrigin(p[0].getSize().x / 2, p[0].getSize().y / 2);
    rozpocznij.setOrigin(pr.getSize().x / 2, pr.getSize().y / 2);
    rozpocznij.setScale((float)window.getSize().y / pr.getSize().x * 0.5, (float)window.getSize().y / pr.getSize().x * 0.5);
    wyjdz.setOrigin(sf::Vector2f(pr.getSize().x / 2, pr.getSize().y / 2));
    wyjdz.setScale((float)window.getSize().y / pr.getSize().x * 0.4, (float)window.getSize().y / pr.getSize().x * 0.4);


    plansza.setPosition(window.getSize().x / 2, window.getSize().y / 2);
    rozpocznij.setPosition(window.getSize().x / 2, window.getSize().y / 2.15);
    wyjdz.setPosition(window.getSize().x / 2, window.getSize().y / 1.5);


    plansza.setScale(1 + (float)window.getSize().y / p[0].getSize().y * 0.98, 1 + (float)window.getSize().y / p[0].getSize().y * 0.98);

}





void Plansza::wyswietlanie()
{
    window.clear();
    window.draw(bg);
    window.draw(podswietlenie);
    window.draw(plansza);

    if (usun_p2 == 0)
    {
        window.draw(wyjdz);
    }

    if (usun_p1 == 0)
    {
        window.draw(rozpocznij);
    }
    if (cala_plansza)
    {

        window.draw(tab);
        window.draw(glosnik);
        window.draw(powrot);
        window.draw(restart);
   
        if(is_move)
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                for (int kk = 0; kk < 4; kk++)
                {
                    if (mozliwe_ruchy[i][j][kk])
                    {
                        if ((pola[i][j][kk].get_x() + pola[i][j][kk].get_y()) % 2)
                        {
                            z_pole.setPosition(pola[i][j][kk].get_wx(), pola[i][j][kk].get_wy());
                            window.draw(z_pole);
                        }
                        else
                        {
                            z_pole1.setPosition(pola[i][j][kk].get_wx(), pola[i][j][kk].get_wy());
                            window.draw(z_pole1);
                        }
                    }


                }
            }
        }
        for (auto i : fig)
            if (i != k)
                window.draw(*i);
   
        window.draw(*k);
        for (auto i = zagrania.begin(); i != zagrania.end(); i++)
            window.draw(*i);

    }
    window.display();
}



void Plansza::przejscia_menu_plansza()
{
    if (i <= 50 && przejscie1)
    {
        plansza.setScale(plansza.getScale().x - 0.02, plansza.getScale().x - 0.02);
        if (i == 10)
        {
            plansza.setTexture(p[2]);
            usun_p1 = 1;
        }
        if (i == 15)
            plansza.setTexture(p[3]);
        if (i == 20)
        {
            plansza.setTexture(p[4]);
            usun_p2 = 1;
        }
        if (i == 25)
            plansza.setTexture(p[5]);
        if (i == 30)
            plansza.setTexture(p[6]);
        if (i == 35)
            plansza.setTexture(p[0]);

        if (i < 10)
            rozpocznij.setScale(rozpocznij.getScale().x * 0.8, rozpocznij.getScale().x * 0.8);
        else if (i < 20)
            wyjdz.setScale(wyjdz.getScale().x * 0.8, wyjdz.getScale().x * 0.8);
        i += 1;
        if (i == 50)
        {
            cala_plansza = 1;
            przejscie1 = 0;
        }
    }

    if (przejscie2 && i >= 0)
    {
        plansza.setScale(plansza.getScale().x + 0.02, plansza.getScale().x + 0.02);
        i -= 1;

        if (i == 15)
            plansza.setTexture(p[1]);
        if (i == 20)
        {
            plansza.setTexture(p[2]);
            usun_p2 = 0;
        }
        if (i == 25)
            plansza.setTexture(p[3]);
        if (i == 30)
            plansza.setTexture(p[4]);
        if (i == 35)
            plansza.setTexture(p[5]);
        if (i == 40)
            plansza.setTexture(p[6]);
        if (i == 10)
            usun_p1 = 0;

        if (i < 10)
            rozpocznij.setScale(rozpocznij.getScale().x * 1.25, rozpocznij.getScale().x * 1.25);
        else if (i < 20)
            wyjdz.setScale(wyjdz.getScale().x * 1.25, wyjdz.getScale().x * 1.25);
        if (i == 0)
        {
            menu = 1;
            przejscie2 = 0;
        }
    }
}



void Plansza::obsluga_menu()
{
    if (rozpocznij.getGlobalBounds().contains(mouse))
    {
        rozpocznij.setScale((float)window.getSize().y / pr.getSize().x * 0.55, (float)window.getSize().y / pr.getSize().x * 0.55);
        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.key.code == sf::Mouse::Left)
            {
                rozpocznij.setScale((float)window.getSize().y / pr.getSize().x * 0.5, (float)window.getSize().y / pr.getSize().x * 0.5);
                menu = 0;
                przejscie1 = 1;
            }
        }
    }
    else
        rozpocznij.setScale((float)window.getSize().y / pr.getSize().x * 0.5, (float)window.getSize().y / pr.getSize().x * 0.5);

    if (wyjdz.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(window)))
    {
        wyjdz.setScale((float)window.getSize().y / pr.getSize().x * 0.45, (float)window.getSize().y / pr.getSize().x * 0.45);
        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.key.code == sf::Mouse::Left)
                window.close();
        }
    }
    else
        wyjdz.setScale((float)window.getSize().y / pr.getSize().x * 0.4, (float)window.getSize().y / pr.getSize().x * 0.4);
}




void Plansza::przyciski_na_planszy()
{
    if (powrot.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(window)))
    {
        powrot.setScale(window.getSize().y * 0.00023, window.getSize().y * 0.00021);
        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.key.code == sf::Mouse::Left)
            {
                powrot.setScale(window.getSize().y * 0.00018, window.getSize().y * 0.00017);
                cala_plansza = 0;
                przejscie2 = 1;
            }
        }
    }
    else
        powrot.setScale(window.getSize().y * 0.00018, window.getSize().y * 0.00017);

    if (restart.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(window)))
    {
        restart.setScale(window.getSize().y * 0.00023, window.getSize().y * 0.00021);
    }
    else
        restart.setScale(window.getSize().y * 0.00018, window.getSize().y * 0.00017);

    if (glosnik.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(window)))
    {
        glosnik.setScale(window.getSize().y * 0.00023, window.getSize().y * 0.00021);
    }
    else
        glosnik.setScale(window.getSize().y * 0.00018, window.getSize().y * 0.00017);

}




void Plansza::przeniesienie_figury()
{
    is_move = 0;
    for (int i1 = 0; i1 < 3; i1++)
    {
        for (int i2 = 0; i2 < 8; i2++)
        {
            for (int i3 = 0; i3 < 4; i3++)
            {
                if (k->get_sprite().getGlobalBounds().contains(pola[i1][i2][i3].get_wx(), pola[i1][i2][i3].get_wy()) && mozliwe_ruchy[i1][i2][i3]==1)
                {

                    k->set_polozenie(pola[i1][i2][i3]);
                    poprawne_przeniesienie = 1;
                    ruch += 1;
                    ruch = ruch % 3;
                    if (ruch == 1)
                        podswietlenie.setTexture(sw_c);
                    else if (ruch == 2)
                        podswietlenie.setTexture(sw);
                    else
                        podswietlenie.setTexture(sw_b);

                    if (zagrania.size() >= 10)
                    {
                        zagrania.pop_front();

                    }
                    for (auto i4 = zagrania.begin(); i4 != zagrania.end(); i4++)
                    {
                        i4->setPosition(i4->getPosition().x, i4->getPosition().y + w_size * 0.02);
                    }

                    text.setString(k->get_nazwa() + "        " + pol_s.get_pole_tab() + "    ->    " + pola[i1][i2][i3].get_pole_tab());

                    text.setOrigin(text.getGlobalBounds().width, 0);
                    if (k->get_kolor() == 0)
                        text.setFillColor(sf::Color::White);
                    else if (k->get_kolor() == 1)
                        text.setFillColor(sf::Color::Black);
                    else
                        text.setFillColor(sf::Color::Red);
                    zagrania.push_back(text);
                }

            }


        }

    }
    if (!poprawne_przeniesienie)
        k->set_polozenie(pol_s);
    poprawne_przeniesienie = 0;
}



void pola_w_tablicy(Pole pola[3][8][4], int sx, int sy, int rozmiar)
{
    for (int i1 = 0; i1 < 3; i1++)
    {
        for (int i2 = 0; i2 < 4; i2++)
        {
            for (int i3 = 0; i3 < 8; i3++)
            {
                pola[i1][i3][i2] = Pole(i3, i2, i1, sx, sy, rozmiar);
            }

        }
    }
}