#include <SFML/Graphics.hpp>
#include <iostream>
#include "Pole.h"
#include <vector>
#include "Figura.h"
#include "Pionek.h"
#include "Wieza.h"
#include "Goniec.h"
#include "Skoczek.h"
#include "Krol.h"
#include "Hetman.h"
#include <list>


int main()
{
    sf::RenderWindow window(sf::VideoMode(16000, 9000), "Szachy dla 3 osób", sf::Style::Fullscreen);
  
   // sf::RenderWindow window1(sf::VideoMode(1200, 1000), "Szachy dla 3 osób");
    window.setFramerateLimit(60);
    int w_size = window.getSize().y;
    sf::Texture tlo;
    tlo.loadFromFile("images/tlo.png");
    sf::Sprite bg(tlo);
    bg.setScale((float)window.getSize().x/tlo.getSize().x, (float)window.getSize().y / tlo.getSize().y);

 
    sf::Texture p, p2, p3, p4, p5, p6, p7;
    p.loadFromFile("images/plansza.png");
    p2.loadFromFile("images/plansza2.png");
    p3.loadFromFile("images/plansza3.png");
    p4.loadFromFile("images/plansza4.png");
    p5.loadFromFile("images/plansza5.png");
    p6.loadFromFile("images/plansza6.png");
    p7.loadFromFile("images/plansza7.png");
    sf::Sprite plansza(p2);
    sf::Vector2f v;

    sf::Texture pr, pw, pp, tabela, g1, g2, res, sw, sw_c, sw_b;
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
    
    sf::Sprite rozpocznij(pr);
    sf::Sprite wyjdz(pw);
    sf::Sprite powrot(pp);
    sf::Sprite tab(tabela);
    sf::Sprite glosnik(g1);
    sf::Sprite restart(res);
    sf::Sprite podswietlenie(sw_b);

    tab.setScale(window.getSize().y*0.0003, window.getSize().y*0.0003);
    tab.setOrigin(tabela.getSize().x, 0);
    tab.setPosition(window.getSize().x*0.99, window.getSize().y*0.01);

    glosnik.setScale(window.getSize().y * 0.00018, window.getSize().y * 0.00017);
    glosnik.setPosition(window.getSize().x * 0.13, window.getSize().y * 0.06);
    glosnik.setOrigin(g1.getSize().x*0.5,g1.getSize().y*0.5 );

    restart.setScale(window.getSize().y * 0.00018, window.getSize().y * 0.00017);
    restart.setPosition(window.getSize().x * 0.08, window.getSize().y * 0.06);
    restart.setOrigin(res.getSize().x * 0.5, res.getSize().y * 0.5);

    powrot.setScale(window.getSize().y * 0.00018, window.getSize().y * 0.00017);
    powrot.setPosition(window.getSize().x * 0.03, window.getSize().y * 0.06);
    powrot.setOrigin(pp.getSize().x * 0.5, pp.getSize().y * 0.5);

    podswietlenie.setOrigin(sw.getSize().x / 2, sw.getSize().y / 2);
    podswietlenie.setPosition(window.getSize().x * 0.5, window.getSize().y * 0.5);
    podswietlenie.setScale(w_size * 0.0005, w_size * 0.0007);

    sf::Font font;
    font.loadFromFile("ARIBL0.ttf");
    std::list<std::string> zagrania();


    Pole pola[3][8][4];
    for (int i1 = 0; i1 < 3; i1++)
    {
        for (int i2 = 0; i2 < 4; i2++)
        {
            for (int i3 = 0; i3 < 8; i3++)
            {
                pola[i1][i3][i2] = Pole(i3, i2,i1, window.getSize().x / 2, window.getSize().y / 2, (float)(window.getSize().y));
            }
 
        }
    }

    std::vector<std::shared_ptr<Figura>> fig;

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

    std::shared_ptr<Figura> krol = std::make_shared<Krol>(pola[0][3][0], w_size);
    fig.push_back(krol);
    krol = std::make_shared<Krol>(pola[1][4][0], w_size);
    fig.push_back(krol);
    krol = std::make_shared<Krol>(pola[2][3][0], w_size);
    fig.push_back(krol);

    std::shared_ptr<Figura> hetman = std::make_shared<Hetman>(pola[0][4][0], w_size);
    fig.push_back(hetman);
    hetman = std::make_shared<Hetman>(pola[1][3][0], w_size);
    fig.push_back(hetman);
    hetman = std::make_shared<Hetman>(pola[2][4][0], w_size);
    fig.push_back(hetman);


  /*  std::shared_ptr<Figura> wiezab = std::make_shared<Wieza>(pola[1][0][0], w_size);
    fig.push_back(wiezab);
    std::shared_ptr<Figura> wiezab1 = std::make_shared<Wieza>(pola[1][0][7], w_size);
    fig.push_back(wiezab1);
    std::shared_ptr<Figura> wiezab = std::make_shared<Wieza>(pola[2][0][0], w_size);
    fig.push_back(wiezab);
    std::shared_ptr<Figura> wiezab1 = std::make_shared<Wieza>(pola[2][0][7], w_size);
    fig.push_back(wiezab1);*/



    
    v.x = p.getSize().x / 2;
    v.y = p.getSize().y / 2;
    plansza.setOrigin(v);
    rozpocznij.setOrigin(sf::Vector2f(pr.getSize().x/2,pr.getSize().y / 2));
    rozpocznij.setScale((float)window.getSize().y/pr.getSize().x*0.5, (float)window.getSize().y / pr.getSize().x * 0.5);
    wyjdz.setOrigin(sf::Vector2f(pr.getSize().x / 2, pr.getSize().y / 2));
    wyjdz.setScale((float)window.getSize().y / pr.getSize().x * 0.4, (float)window.getSize().y / pr.getSize().x * 0.4);
   // powrot.setOrigin(sf::Vector2f(pp.getSize().x / 2, pp.getSize().y / 2));
  //  powrot.setScale((float)window.getSize().y / pp.getSize().x * 0.2, (float)window.getSize().y / pp.getSize().x * 0.2);
   /* float py = p.getSize().y;
    rozpocznij.setPosition(0, -py*0.01);*/

    plansza.setPosition(window.getSize().x/2, window.getSize().y/2);
    rozpocznij.setPosition(window.getSize().x / 2, window.getSize().y / 2.15);
    wyjdz.setPosition(window.getSize().x / 2, window.getSize().y / 1.5);

   // powrot.setPosition(window.getSize().x *0.068, window.getSize().y*0.05);
   
    plansza.setScale(1+(float)window.getSize().y / p.getSize().y*0.98, 1+(float)window.getSize().y / p.getSize().y*0.98);

   /* float rozpocznij_scale = rozpocznij.getScale().x;
    float wyjdz_scale = rozpocznij.getScale().x;*/

    sf::Event event;
  
    int i = 0;
    bool usun_p1 = 0,usun_p2=0, menu=1, cala_plansza=0, przejscie1=0, przejscie2=0;
    std::shared_ptr<Figura> k=fig[0];
    bool is_move = 0, poprawne_przeniesienie=0;
    Pole pol_s;
    float dx, dy;
    int ruch = 0;

    while (window.isOpen())
    {
      
        sf::Vector2f mouse = (sf::Vector2f)sf::Mouse::getPosition(window);

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (menu == 1)
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
            if(cala_plansza)
            {
                if (powrot.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(window)))
                {
                    powrot.setScale(window.getSize().y * 0.00023, window.getSize().y * 0.00021);
                    if (event.type == sf::Event::MouseButtonPressed)
                    {
                        if (event.key.code == sf::Mouse::Left)
                        {
                            powrot.setScale(window.getSize().y * 0.00018, window.getSize().y * 0.00017);
                            /*menu = 1;*/cala_plansza = 0;
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

            if (event.type == sf::Event::MouseButtonPressed)
                if (event.key.code == sf::Mouse::Left)
                    for (auto i : fig)
                        if (i->get_sprite().getGlobalBounds().contains(mouse)&&i->get_kolor() == ruch)
                        {
                            pol_s = i->get_pole();
                            k = i;
                            is_move = 1;
                            dx = k->get_pole().get_wx() - mouse.x;
                            dy = k->get_pole().get_wy() - mouse.y;
                        }
            

            if (event.type == sf::Event::MouseButtonReleased&&is_move)
                if (event.key.code == sf::Mouse::Left)
                {
                    is_move = 0;
                    for (int i1 = 0; i1 < 3; i1++)
                    {
                        for (int i2 = 0; i2 < 8; i2++)
                        {
                            for (int i3 = 0; i3 < 4; i3++)
                            {
                                if (k->get_sprite().getGlobalBounds().contains(pola[i1][i2][i3].get_wx(), pola[i1][i2][i3].get_wy()))
                                {
                                    std::cout << 1;
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
                                }

                            }


                        }
                    
                    }
                    if (!poprawne_przeniesienie)
                        k->set_polozenie(pol_s);
                    poprawne_przeniesienie = 0;
                }
     
        }
    
        if (is_move)
        {
            k->set_position(mouse.x+dx, mouse.y+dy);
        }
      
        if (/*menu == 0*/ i <= 50&&przejscie1)
        {
            plansza.setScale(plansza.getScale().x - 0.02, plansza.getScale().x - 0.02);
           // i += 1;
            if (i == 10)
            {
                plansza.setTexture(p3);
                usun_p1 = 1;
            }
            if (i == 15)
            {
                plansza.setTexture(p4);
            }
            if (i == 20)
            {
                plansza.setTexture(p5);
                usun_p2 = 1;
            }
            if (i == 25)
                plansza.setTexture(p6);
            if (i == 30)
                plansza.setTexture(p7);
            if (i == 35)
                plansza.setTexture(p);

            if (i < 10)
                rozpocznij.setScale(rozpocznij.getScale().x *0.8, rozpocznij.getScale().x *0.8);
            else if (i < 20)
                wyjdz.setScale(wyjdz.getScale().x *0.8, wyjdz.getScale().x *0.8);
            i += 1;
            if (i == 50)
            {
                cala_plansza = 1;
                przejscie1 = 0;
            }
        }

        if (przejscie2 && i >=0)
        {
            plansza.setScale(plansza.getScale().x + 0.02, plansza.getScale().x + 0.02);
            i -= 1;
    
            if (i == 15)
            {
                plansza.setTexture(p2);
               
            }
            if (i == 20)
            {
                plansza.setTexture(p3);
                usun_p2 = 0;
            }
            if (i == 25)
            {
                plansza.setTexture(p4);
                
            }
            
            if (i == 30)
                plansza.setTexture(p5);
            if (i == 35)
                plansza.setTexture(p6);
            if (i == 40)
                plansza.setTexture(p7);
            if (i == 10)
            {
 
                usun_p1 = 0;
            }
            if (i < 10)
                rozpocznij.setScale(rozpocznij.getScale().x *1.25, rozpocznij.getScale().x *1.25);
            else if (i < 20)
            {
                wyjdz.setScale(wyjdz.getScale().x *1.25, wyjdz.getScale().x *1.25);
            }
            if (i == 0)
            {
                menu = 1;
                przejscie2 = 0;
            }
           // cala_plansza = 0;
        }

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::MouseButtonPressed)
                if (event.key.code == sf::Mouse::Left)
                    for (auto i : fig)
                        if (i->get_sprite().getGlobalBounds().contains(mouse))
                        {
                            Pole pol_s = i->get_pole();
                            k = i;
                            i->set_position(mouse.x, mouse.y);
                        }
        }
     

        window.clear();
        window.draw(bg);
        window.draw(podswietlenie);
        window.draw(plansza);
      //  window.draw(podswietlenie);
        
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
            for(auto i : fig)
                if(i!=k)
                   window.draw(*i);
            window.draw(*k);

        }
        window.display();
    }
    
    return 0;
    
}

