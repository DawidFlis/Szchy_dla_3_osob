#include <SFML/Graphics.hpp>
#include <iostream>
#include "Pole.h"
#include <vector>

int main()
{
    sf::RenderWindow window(sf::VideoMode(16000, 9000), "Szachy dla 3 osób", sf::Style::Fullscreen);
  
   // sf::RenderWindow window1(sf::VideoMode(1200, 1000), "Szachy dla 3 osób");
    window.setFramerateLimit(60);
    
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

    sf::Texture pr, pw, pp;
    pr.loadFromFile("images/rozpocznij2.png");
    pw.loadFromFile("images/wyjdz.png");
    pp.loadFromFile("images/powrot.png");
    sf::Sprite rozpocznij(pr);
    sf::Sprite wyjdz(pw);
    sf::Sprite powrot(pp);

    std::vector<sf::Sprite> fig;
    sf::Texture pio;
    pio.loadFromFile("images/pion_r.png");
    for(int k=0;k<3;k++)
    for (int i = 0; i < 8; i++)
        for(int j=0;j<2;j++)
    {
        sf::Sprite pion(pio);
        Pole pol(i, j, k, window.getSize().x / 2, window.getSize().y / 2, (float)(window.getSize().y));
        pion.setOrigin(pio.getSize().x / 2, pio.getSize().y / 2);
        pion.setScale(0.4, 0.4);
        pion.setPosition(pol.get_wx(), pol.get_wy());
        fig.push_back(pion);
    }
    
    v.x = p.getSize().x / 2;
    v.y = p.getSize().y / 2;
    plansza.setOrigin(v);
    rozpocznij.setOrigin(sf::Vector2f(pr.getSize().x/2,pr.getSize().y / 2));
    rozpocznij.setScale((float)window.getSize().y/pr.getSize().x*0.5, (float)window.getSize().y / pr.getSize().x * 0.5);
    wyjdz.setOrigin(sf::Vector2f(pr.getSize().x / 2, pr.getSize().y / 2));
    wyjdz.setScale((float)window.getSize().y / pr.getSize().x * 0.4, (float)window.getSize().y / pr.getSize().x * 0.4);
    powrot.setOrigin(sf::Vector2f(pp.getSize().x / 2, pp.getSize().y / 2));
    powrot.setScale((float)window.getSize().y / pp.getSize().x * 0.2, (float)window.getSize().y / pp.getSize().x * 0.2);
   /* float py = p.getSize().y;
    rozpocznij.setPosition(0, -py*0.01);*/

    plansza.setPosition(window.getSize().x/2, window.getSize().y/2);
    rozpocznij.setPosition(window.getSize().x / 2, window.getSize().y / 2.15);
    wyjdz.setPosition(window.getSize().x / 2, window.getSize().y / 1.5);

    powrot.setPosition(window.getSize().x *0.068, window.getSize().y*0.05);
   
    plansza.setScale(1+(float)window.getSize().y / p.getSize().y*0.98, 1+(float)window.getSize().y / p.getSize().y*0.98);

   /* float rozpocznij_scale = rozpocznij.getScale().x;
    float wyjdz_scale = rozpocznij.getScale().x;*/

    sf::Event event;
  
    int i = 0;
    bool usun_p1 = 0,usun_p2=0, menu=1, cala_plansza=0;

    while (window.isOpen())
    {
      


        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (menu == 1)
            {
                if (rozpocznij.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(window)))
                {
                    rozpocznij.setScale((float)window.getSize().y / pr.getSize().x * 0.55, (float)window.getSize().y / pr.getSize().x * 0.55);
                    if (event.type == sf::Event::MouseButtonPressed)
                    {
                        if (event.key.code == sf::Mouse::Left)
                            menu = 0;
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
            else
            {
                if (powrot.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(window)))
                {
                    powrot.setScale((float)window.getSize().y / pp.getSize().x * 0.23, (float)window.getSize().y / pp.getSize().x * 0.23);
                    if (event.type == sf::Event::MouseButtonPressed)
                    {
                        if (event.key.code == sf::Mouse::Left)
                            menu = 1;
                    }
                }
                else
                    powrot.setScale((float)window.getSize().y / pp.getSize().x * 0.2, (float)window.getSize().y / pp.getSize().x * 0.2);
            }
           
        }
      
        if (menu == 0 &&i < 50)
        {
            plansza.setScale(plansza.getScale().x - 0.02, plansza.getScale().x - 0.02);
            i += 1;
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
                rozpocznij.setScale(rozpocznij.getScale().x * 0.7, rozpocznij.getScale().x * 0.7);
            else if (i < 20)
                wyjdz.setScale(wyjdz.getScale().x * 0.7, wyjdz.getScale().x * 0.7);

            if (i == 49)
                cala_plansza = 1;
        }

        if (menu == 1 && i > 0)
        {
            plansza.setScale(plansza.getScale().x + 0.02, plansza.getScale().x + 0.02);
            i -= 1;
            if (i == 10)
            {
                plansza.setTexture(p3);
                usun_p2 = 0;
                //   rozpocznij.setScale((float)window.getSize().y / pr.getSize().x * 0.5, (float)window.getSize().y / pr.getSize().x * 0.5);
                 //  wyjdz.setScale((float)window.getSize().y / pr.getSize().x * 0.4, (float)window.getSize().y / pr.getSize().x * 0.4);
            }
            if (i == 15)
            {
                plansza.setTexture(p4);
            }
            if (i == 20)
                plansza.setTexture(p5);
            
            if (i == 25)
                plansza.setTexture(p6);
            if (i == 30)
                plansza.setTexture(p7);
            if (i == 1)
            {
                plansza.setTexture(p2);
                usun_p1 = 0;
            }
     
            cala_plansza = 0;
        }
     

        window.clear();
        window.draw(bg);
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
            window.draw(powrot);
            for(int k=0;k<48;k++)
                window.draw(fig[k]);
        }
       // window.draw(pion);
        window.display();
    }
    
    return 0;
    
}

