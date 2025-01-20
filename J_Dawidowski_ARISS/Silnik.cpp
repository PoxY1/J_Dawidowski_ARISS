#include "Silnik.h"
#include <iostream>
#include <sstream>

SilnikGry::SilnikGry()
    : okno(sf::VideoMode(SZEROKOSC_OKNA, WYSOKOSC_OKNA), "Pac-Man wersja PG"),
    etap(EtapGry::MENU),
    postac(nullptr),
    indeksPoziomu(0),
    punkty(0),
    nicknameEntered(false)
{
    okno.setFramerateLimit(60);

    if (!czcionka.loadFromFile("CSMS.ttf"))
        std::cerr << "Blad ladowania czcionki CSMS.ttf!\n";

    napisStatus.setFont(czcionka);
    napisStatus.setCharacterSize(26);
    napisStatus.setFillColor(sf::Color::Yellow);
    napisStatus.setOutlineColor(sf::Color::Blue);
    napisStatus.setOutlineThickness(3.f);
    napisStatus.setPosition(20.f, 20.f);

    napisPomoc.setFont(czcionka);
    napisPomoc.setCharacterSize(24);
    napisPomoc.setFillColor(sf::Color(50, 200, 50));
    napisPomoc.setOutlineColor(sf::Color::Black);
    napisPomoc.setOutlineThickness(2.f);
    napisPomoc.setPosition(150.f, 100.f);
    napisPomoc.setString(
        "POMOC:\n"
        "[Strzalki] - ruch\n"
        "Sciany nie do przejscia\n"
        "[ESC] - Wyjscie (T/N)\n"
        "[F1] - pauza/powrot\n"
    );

    napisGameOver.setFont(czcionka);
    napisGameOver.setCharacterSize(60);
    napisGameOver.setFillColor(sf::Color::Red);
    napisGameOver.setOutlineColor(sf::Color::Black);
    napisGameOver.setOutlineThickness(3.f);
    napisGameOver.setString("GAME OVER!\n[Enter] - ponownie");
    napisGameOver.setPosition(250.f, 250.f);

    napisNick.setFont(czcionka);
    napisNick.setCharacterSize(32);
    napisNick.setFillColor(sf::Color::Red);
    napisNick.setOutlineColor(sf::Color::Black);
    napisNick.setOutlineThickness(3.f);
    //napisNick.setString("GAME OVER!\n[Enter] - ponownie");
    napisNick.setPosition(50.f, 50.f);

    tlo.setTexture(zasoby.wezTloLabirynt());

    inicjujPoziomy();
}

SilnikGry::~SilnikGry()
{
    delete postac;
    for (auto& p : poziomy)
    {
        for (auto d : p.duchy)          delete d;
        for (auto n : p.sciankiNiereg)  delete n;
        for (auto s : p.sciankiProst)   delete s;
        for (auto j : p.jablka)         delete j;
    }
}

void SilnikGry::uruchom()
{
    // Tworzymy postac
    postac = new Postac(zasoby.wezTexPostac());
    indeksPoziomu = 0;

    while (okno.isOpen() && etap != EtapGry::KONIEC)
    {
        float dt = zegar.restart().asSeconds();
        petlaZdarzen();
        aktualizuj(dt);
        rysuj();
    }
}

void SilnikGry::zapytajNick(const sf::Event& event)
{

    if (event.type == sf::Event::TextEntered)
    {
        std::cout << aktualnyNick << "\n";
        if (event.text.unicode >= 32 && event.text.unicode < 128)
        {
            aktualnyNick += static_cast<char>(event.text.unicode);
            std::cout << aktualnyNick << "\n";
        }
    }
    else if (event.type == sf::Event::KeyPressed)
    {
        switch (event.key.code)
        {
        case sf::Keyboard::Enter:
        {
            // Dodajemy gracza
            GraczRanking pd;

            pd.nick = aktualnyNick;
            pd.punkty = 0;
            gracze.push_back(pd);

            std::sort(gracze.begin(), gracze.end(),
                [](const GraczRanking& a, const GraczRanking& b) { return a.punkty > b.punkty; }
            );

            // checkowanieczy wpisany nick juz
            nicknameEntered = true;

            postac->ustawPozycje(SZEROKOSC_OKNA / 3.f, WYSOKOSC_OKNA / 3.f);
            etap = EtapGry::ROZGRYWKA;
        }
        break;
        case sf::Keyboard::BackSpace:
        {
            if (!aktualnyNick.empty()) aktualnyNick.pop_back();
        }
        break;
        case sf::Keyboard::Escape:
        {
            etap = EtapGry::MENU;
        }
        break;
        default:
            break;
        }
    }
}

std::vector<sf::FloatRect> SilnikGry::pobierzScianyAktPoziomu() const
{
    std::vector<sf::FloatRect> ret;
    if (indeksPoziomu < 0 || indeksPoziomu >= (int)poziomy.size()) return ret;

    auto& lvl = poziomy[indeksPoziomu];
    for (auto n : lvl.sciankiNiereg)
        ret.push_back(n->pobierzGranice());
    for (auto s : lvl.sciankiProst)
        ret.push_back(s->pobierzGranice());

    return ret;
}

void SilnikGry::inicjujPoziomy()
{
    poziomy.resize(5);

    // Poziom 1
    {

        auto d = new Przeciwnik(zasoby.wezTexPrzeciwnik());
        d->ustawPozycje(500.f, 200.f);
        poziomy[0].duchy.push_back(d);
        

        auto ni1 = new PrzeszkodaNieregularna({ 100.f,300.f }, sf::Color::Green);
        auto ni2 = new PrzeszkodaNieregularna({ 700.f,350.f }, sf::Color::Magenta);
        auto ni3 = new PrzeszkodaNieregularna({ 920.f,350.f }, sf::Color::Cyan);
        auto ni4 = new PrzeszkodaNieregularna({ 400.f,650.f }, sf::Color::Yellow);
        poziomy[0].sciankiNiereg.push_back(ni1);
        poziomy[0].sciankiNiereg.push_back(ni2);
        poziomy[0].sciankiNiereg.push_back(ni3);
        poziomy[0].sciankiNiereg.push_back(ni4);

        auto pr1 = new PrzeszkodaProstokatna({ 0.f,70.f }, { 1280.f,20.f }, sf::Color::Blue); //top border
        auto pr2= new PrzeszkodaProstokatna({ 100.f, 90.f }, { 20.f,550.f }, sf::Color::Blue);//
        auto pr3 = new PrzeszkodaProstokatna({ 120.f, 550.f }, { 500.f,20.f }, sf::Color::Blue);//
        auto pr4 = new PrzeszkodaProstokatna({ 620.f, 550.f }, { 20.f,90.f }, sf::Color::Blue);//
        auto pr5 = new PrzeszkodaProstokatna({ 215.f, 200.f }, { 20.f,250.f }, sf::Color::Blue);//
        auto pr6 = new PrzeszkodaProstokatna({ 215.f, 450.f }, { 400.f,20.f }, sf::Color::Blue);//  
        auto pr7 = new PrzeszkodaProstokatna({ 615.f, 70.f }, { 20.f,400.f }, sf::Color::Blue);//
        auto pr8 = new PrzeszkodaProstokatna({ 800.f, 170.f }, { 20.f,280.f }, sf::Color::Blue);//
        auto pr9 = new PrzeszkodaProstokatna({ 800.f, 170.f }, { 200.f,20.f }, sf::Color::Blue);//
        auto pr10 = new PrzeszkodaProstokatna({ 800.f, 450.f }, { 200.f,20.f }, sf::Color::Blue);//
        auto pr11 = new PrzeszkodaProstokatna({ 1150.f, 170.f }, { 20.f,400.f }, sf::Color::Blue);//
        auto pr12 = new PrzeszkodaProstokatna({ 1150.f, 170.f }, { 20.f,400.f }, sf::Color::Blue);//

        poziomy[0].sciankiProst.push_back(pr1);
        poziomy[0].sciankiProst.push_back(pr2);
        poziomy[0].sciankiProst.push_back(pr3);
        poziomy[0].sciankiProst.push_back(pr4);
        poziomy[0].sciankiProst.push_back(pr5);
        poziomy[0].sciankiProst.push_back(pr6);
        poziomy[0].sciankiProst.push_back(pr7);
        poziomy[0].sciankiProst.push_back(pr8);
        poziomy[0].sciankiProst.push_back(pr9);
        poziomy[0].sciankiProst.push_back(pr10);
        poziomy[0].sciankiProst.push_back(pr11);
        poziomy[0].sciankiProst.push_back(pr12);



        auto j1 = new Jablko(zasoby.wezTexJablko(), { 50.f,150.f });
        auto j2 = new Jablko(zasoby.wezTexJablko(), { 750.f,300.f });
        auto j3 = new Jablko(zasoby.wezTexJablko(), { 500.f,350.f });
        auto j4 = new Jablko(zasoby.wezTexJablko(), { 160.f,600.f });
        auto j5 = new Jablko(zasoby.wezTexJablko(), { 900.f,300.f });

        poziomy[0].jablka.push_back(j1);
        poziomy[0].jablka.push_back(j2);
        poziomy[0].jablka.push_back(j3);
        poziomy[0].jablka.push_back(j4);
        poziomy[0].jablka.push_back(j5);
    }
    // Poziom 2
    {
        auto d1 = new Przeciwnik(zasoby.wezTexPrzeciwnik());
        d1->ustawPozycje(200.f, 200.f);
        auto d2 = new Przeciwnik(zasoby.wezTexPrzeciwnik());
        d2->ustawPozycje(900.f, 500.f);
        poziomy[1].duchy.push_back(d1);
        poziomy[1].duchy.push_back(d2);

        auto n1 = new PrzeszkodaNieregularna({ 300.f,300.f }, sf::Color::Blue);
        auto n2 = new PrzeszkodaNieregularna({ 1000.f,350.f }, sf::Color::Magenta);
        auto n3 = new PrzeszkodaNieregularna({ 100.f,600.f }, sf::Color::Yellow);

        poziomy[1].sciankiNiereg.push_back(n1);
        poziomy[1].sciankiNiereg.push_back(n2);
        poziomy[1].sciankiNiereg.push_back(n3);

        auto p1 = new PrzeszkodaProstokatna({ 0.f,70.f }, { 1280.f,20.f }, sf::Color::Green); //top border
        auto p2 = new PrzeszkodaProstokatna({ 0.f,300.f }, { 1000.f,20.f }, sf::Color::Green);//
        auto p3 = new PrzeszkodaProstokatna({ 280.f,500.f }, { 1000.f,20.f }, sf::Color::Green);//
        auto p4 = new PrzeszkodaProstokatna({ 0.f,700.f }, { 1000.f,20.f }, sf::Color::Green);//

        poziomy[1].sciankiProst.push_back(p1);
        poziomy[1].sciankiProst.push_back(p2);
        poziomy[1].sciankiProst.push_back(p3);
        poziomy[1].sciankiProst.push_back(p4);

        auto jb1 = new Jablko(zasoby.wezTexJablko(), { 800.f,190.f });
        auto jb2 = new Jablko(zasoby.wezTexJablko(), { 800.f,400.f });
        auto jb3 = new Jablko(zasoby.wezTexJablko(), { 200.f,400.f });
        auto jb4 = new Jablko(zasoby.wezTexJablko(), { 800.f,600.f });
        auto jb5 = new Jablko(zasoby.wezTexJablko(), { 100.f,190.f });

        poziomy[1].jablka.push_back(jb1);
        poziomy[1].jablka.push_back(jb2);
        poziomy[1].jablka.push_back(jb3);
        poziomy[1].jablka.push_back(jb4);
        poziomy[1].jablka.push_back(jb5);
    }
    // Poziom 3
    {
        auto du1 = new Przeciwnik(zasoby.wezTexPrzeciwnik());
        auto du2 = new Przeciwnik(zasoby.wezTexPrzeciwnik());
        auto du3 = new Przeciwnik(zasoby.wezTexPrzeciwnik());
        du1->ustawPozycje(125.f, 200.f);
        du2->ustawPozycje(625.f, 520.f);
        du3->ustawPozycje(875.f, 200.f);

        poziomy[2].duchy.push_back(du1);
        poziomy[2].duchy.push_back(du2);
        poziomy[2].duchy.push_back(du3);


        auto nr1 = new PrzeszkodaNieregularna({ 300.f,300.f }, sf::Color::Blue);
        auto nr2 = new PrzeszkodaNieregularna({ 450.f,600.f }, sf::Color::Green);
        auto nr3 = new PrzeszkodaNieregularna({ 700.f,200.f }, sf::Color::Magenta);
        poziomy[2].sciankiNiereg.push_back(nr1);
        poziomy[2].sciankiNiereg.push_back(nr2);
        poziomy[2].sciankiNiereg.push_back(nr3);


        auto pt1 = new PrzeszkodaProstokatna({ 0.f,70.f }, { 1280.f,20.f }, sf::Color::Red); //top border
        auto pt2 = new PrzeszkodaProstokatna({ 250.f,70.f }, { 20.f,550.f }, sf::Color::Red);
        auto pt3 = new PrzeszkodaProstokatna({ 500.f,170.f }, { 20.f,550.f }, sf::Color::Red);
        auto pt4 = new PrzeszkodaProstokatna({ 750.f,70.f }, { 20.f,550.f }, sf::Color::Red);
        auto pt5 = new PrzeszkodaProstokatna({ 1000.f,170.f }, { 20.f,550.f }, sf::Color::Red);
  
        poziomy[2].sciankiProst.push_back(pt1);
        poziomy[2].sciankiProst.push_back(pt2);
        poziomy[2].sciankiProst.push_back(pt3);
        poziomy[2].sciankiProst.push_back(pt4);
        poziomy[2].sciankiProst.push_back(pt5);


        auto ja1 = new Jablko(zasoby.wezTexJablko(), { 125.f,400.f });
        auto ja2 = new Jablko(zasoby.wezTexJablko(), { 625.f,400.f });
        auto ja3 = new Jablko(zasoby.wezTexJablko(), { 875.f,400.f });
        auto ja4 = new Jablko(zasoby.wezTexJablko(), { 1125.f,400.f });
        auto ja5 = new Jablko(zasoby.wezTexJablko(), { 375.f,400.f });

        poziomy[2].jablka.push_back(ja1);
        poziomy[2].jablka.push_back(ja2);
        poziomy[2].jablka.push_back(ja3);
        poziomy[2].jablka.push_back(ja4);
        poziomy[2].jablka.push_back(ja5);

    }
    // Poziom 4
    {
        auto da1 = new Przeciwnik(zasoby.wezTexPrzeciwnik());

        da1->ustawPozycje(125.f, 200.f);

        poziomy[3].duchy.push_back(da1);



        auto ne1 = new PrzeszkodaNieregularna({ 300.f,150 }, sf::Color::Blue);
        auto ne2 = new PrzeszkodaNieregularna({ 400,630 }, sf::Color::Yellow);
        auto ne3 = new PrzeszkodaNieregularna({ 500,233 }, sf::Color::Red);
        auto ne4 = new PrzeszkodaNieregularna({ 600,111 }, sf::Color::White);
        auto ne5 = new PrzeszkodaNieregularna({ 700.f,589 }, sf::Color::Green);
        auto ne6 = new PrzeszkodaNieregularna({ 800,359 }, sf::Color::Blue);
        auto ne7 = new PrzeszkodaNieregularna({ 900,690 }, sf::Color::Yellow);
        auto ne8 = new PrzeszkodaNieregularna({ 1000,250 }, sf::Color::Red);
        auto ne9 = new PrzeszkodaNieregularna({ 200,530 }, sf::Color::White);

        poziomy[3].sciankiNiereg.push_back(ne1);
        poziomy[3].sciankiNiereg.push_back(ne2);
        poziomy[3].sciankiNiereg.push_back(ne3);
        poziomy[3].sciankiNiereg.push_back(ne4);
        poziomy[3].sciankiNiereg.push_back(ne5);
        poziomy[3].sciankiNiereg.push_back(ne6);
        poziomy[3].sciankiNiereg.push_back(ne7);
        poziomy[3].sciankiNiereg.push_back(ne8);
        poziomy[3].sciankiNiereg.push_back(ne9);




        auto po1 = new PrzeszkodaProstokatna({ 0.f,70.f }, { 1280.f,20.f }, sf::Color::Magenta); //top border
        auto po2 = new PrzeszkodaProstokatna({ 150.f,425.f }, { 980.f,20.f }, sf::Color::Magenta);
        auto po3 = new PrzeszkodaProstokatna({ 150,200.f }, { 20.f,425.f }, sf::Color::Magenta);
        auto po4 = new PrzeszkodaProstokatna({ 1130.f,200.f }, { 20.f,425.f }, sf::Color::Magenta);
        poziomy[3].sciankiProst.push_back(po1);
        poziomy[3].sciankiProst.push_back(po2);
        poziomy[3].sciankiProst.push_back(po3);
        poziomy[3].sciankiProst.push_back(po4);



        auto jc1 = new Jablko(zasoby.wezTexJablko(), { 125.f,400.f });

        poziomy[3].jablka.push_back(jc1);
    }
    // Poziom 5
    {
        auto dh1 = new Przeciwnik(zasoby.wezTexPrzeciwnik());
        auto dh2 = new Przeciwnik(zasoby.wezTexPrzeciwnik());
        auto dh3 = new Przeciwnik(zasoby.wezTexPrzeciwnik());
        auto dh4 = new Przeciwnik(zasoby.wezTexPrzeciwnik());
        auto dh5 = new Przeciwnik(zasoby.wezTexPrzeciwnik());

        dh1->ustawPozycje(125.f, 200.f);
        dh2->ustawPozycje(125.f, 200.f);
        dh3->ustawPozycje(125.f, 200.f);
        dh4->ustawPozycje(125.f, 200.f);
        dh5->ustawPozycje(125.f, 200.f);

        poziomy[4].duchy.push_back(dh1);
        poziomy[4].duchy.push_back(dh2);
        poziomy[4].duchy.push_back(dh3);
        poziomy[4].duchy.push_back(dh4);
        poziomy[4].duchy.push_back(dh5);
    



        auto np1 = new PrzeszkodaNieregularna({ 1095,409 }, sf::Color::Blue);
        auto np2 = new PrzeszkodaNieregularna({ 353,539 }, sf::Color::Yellow);
        auto np3 = new PrzeszkodaNieregularna({ 381,337 }, sf::Color::Red);
        auto np4 = new PrzeszkodaNieregularna({ 517,667 }, sf::Color::White);
        auto np5 = new PrzeszkodaNieregularna({ 140,411 }, sf::Color::Green);
        auto np6 = new PrzeszkodaNieregularna({ 617,633 }, sf::Color::Blue);
        auto np7 = new PrzeszkodaNieregularna({ 200,585 }, sf::Color::Yellow);
        auto np8 = new PrzeszkodaNieregularna({ 830,306 }, sf::Color::Red);
        auto np9 = new PrzeszkodaNieregularna({ 1088,678 }, sf::Color::White);
        auto np10 = new PrzeszkodaNieregularna({ 409,142 }, sf::Color::Green);
        auto np11 = new PrzeszkodaNieregularna({ 111,690 }, sf::Color::Blue);
        auto np12 = new PrzeszkodaNieregularna({ 759.f,681 }, sf::Color::Yellow);
        auto np13 = new PrzeszkodaNieregularna({ 289,226 }, sf::Color::Red);
        auto np14 = new PrzeszkodaNieregularna({ 700,208 }, sf::Color::White);
        auto np15 = new PrzeszkodaNieregularna({ 1156,309 }, sf::Color::Green);
        auto np16 = new PrzeszkodaNieregularna({ 103,111 }, sf::Color::Blue);

        poziomy[4].sciankiNiereg.push_back(np1);
        poziomy[4].sciankiNiereg.push_back(np2);
        poziomy[4].sciankiNiereg.push_back(np3);
        poziomy[4].sciankiNiereg.push_back(np4);
        poziomy[4].sciankiNiereg.push_back(np5);
        poziomy[4].sciankiNiereg.push_back(np6);
        poziomy[4].sciankiNiereg.push_back(np7);
        poziomy[4].sciankiNiereg.push_back(np8);
        poziomy[4].sciankiNiereg.push_back(np9);
        poziomy[4].sciankiNiereg.push_back(np10);
        poziomy[4].sciankiNiereg.push_back(np11);
        poziomy[4].sciankiNiereg.push_back(np12);
        poziomy[4].sciankiNiereg.push_back(np13);
        poziomy[4].sciankiNiereg.push_back(np14);
        poziomy[4].sciankiNiereg.push_back(np15);
        poziomy[4].sciankiNiereg.push_back(np16);



        auto pa1 = new PrzeszkodaProstokatna({ 0.f,70.f }, { 1280.f,20.f }, sf::Color::Yellow); //top border
        auto pa2 = new PrzeszkodaProstokatna({ 250.f,170.f }, { 20.f,450.f }, sf::Color::Yellow);
        auto pa3 = new PrzeszkodaProstokatna({ 500.f,170.f }, { 20.f,450.f }, sf::Color::Yellow);
        auto pa4 = new PrzeszkodaProstokatna({ 750.f,170.f }, { 20.f,450.f }, sf::Color::Yellow);
        auto pa5 = new PrzeszkodaProstokatna({ 1000.f,170.f }, { 20.f,450.f }, sf::Color::Yellow);

        poziomy[4].sciankiProst.push_back(pa1);
        poziomy[4].sciankiProst.push_back(pa2);
        poziomy[4].sciankiProst.push_back(pa3);
        poziomy[4].sciankiProst.push_back(pa4);
        poziomy[4].sciankiProst.push_back(pa5);



        auto jd1 = new Jablko(zasoby.wezTexJablko(), { 55.f,100.f });
        auto jd2 = new Jablko(zasoby.wezTexJablko(), { 222,550.f });
        auto jd3 = new Jablko(zasoby.wezTexJablko(), { 444,300.f });
        auto jd4 = new Jablko(zasoby.wezTexJablko(), { 666,250.f });
        auto jd5 = new Jablko(zasoby.wezTexJablko(), { 888,400.f });

        poziomy[4].jablka.push_back(jd1);
        poziomy[4].jablka.push_back(jd2);
        poziomy[4].jablka.push_back(jd3);
        poziomy[4].jablka.push_back(jd4);
        poziomy[4].jablka.push_back(jd5);
    }
}

void SilnikGry::petlaZdarzen()
{
    sf::Event e;
    while (okno.pollEvent(e))
    {
        if (e.type == sf::Event::Closed)
            etap = EtapGry::WYJSCIE_POTWIERDZENIE;

        if (e.type == sf::Event::KeyPressed)
        {
            auto code = e.key.code;
            if (code == sf::Keyboard::Escape)
                etap = EtapGry::WYJSCIE_POTWIERDZENIE;
            else if (code == sf::Keyboard::F1)
            {
                if (etap == EtapGry::ROZGRYWKA)
                    etap = EtapGry::POMOC;
                else if (etap == EtapGry::POMOC)
                    etap = EtapGry::ROZGRYWKA;
            }
            else if (code == sf::Keyboard::R)
            {

                if (etap == EtapGry::MENU)
                    etap = EtapGry::RANKING;
                else if (etap == EtapGry::RANKING)
                    etap = EtapGry::MENU;
            }
            else if (etap == EtapGry::NICK)
            {
                zapytajNick(e);
                continue;
            }
            else if (etap == EtapGry::MENU && code == sf::Keyboard::Enter)
            {
                if (!nicknameEntered)
                {
                    aktualnyNick.clear();
                    etap = EtapGry::NICK;
                }
                else
                {
                    etap = EtapGry::ROZGRYWKA;
                    punkty = 0;
                    indeksPoziomu = 0;
                    postac->ustawPozycje(SZEROKOSC_OKNA / 3.f, WYSOKOSC_OKNA / 3.f);
                }
                break;
            }
            else if (etap == EtapGry::GAME_OVER && code == sf::Keyboard::Enter)
            {
                // reset
                postac->ustawWynik(indeksPoziomu + 1);
                punkty = 0;
                indeksPoziomu = 0;
                postac->ustawPozycje(SZEROKOSC_OKNA / 3.f, WYSOKOSC_OKNA / 3.f);
                postac->resetujKolizjeZduchem();
                auto& p = poziomy[indeksPoziomu];
                
                    for (auto j : p.jablka) {
                        j->resetujZebranie();
                    }
                    for (auto ja : p.jablka) {
                        ja->resetujZebranie();
                    }
                    for (auto jb : p.jablka) {
                        jb->resetujZebranie();
                    }
                    for (auto jc : p.jablka) {
                        jc->resetujZebranie();
                    }
                    for (auto jd : p.jablka) {
                        jd->resetujZebranie();
                    }

                etap = EtapGry::ROZGRYWKA;

            }
        }
    }
}

void SilnikGry::aktualizuj(float dt)
{
    switch (etap)
    {
    case EtapGry::MENU:
        napisStatus.setString("MENU:\n[Enter] Start\n[R] Ranking\n[ESC] Wyjscie");
        break;
    case EtapGry::NICK:
    {
        std::ostringstream ss;
        ss << "Podaj nick: \n"
            << aktualnyNick
            << "\n[ENTER] - zatwierdz, [ESC] - Anuluj";
        napisNick.setString(ss.str());  
    }
        break;
    case EtapGry::RANKING:
    {
        std::ostringstream ss;
        ss << "TABLICA WYNIKOW:\n\n";
        for (size_t i = 0; i < gracze.size(); i++)
        {
            ss << (i + 1) << ". " << gracze[i].nick
                << " - " << gracze[i].punkty << "\n";
        }
        ss << "\n[ENTER] - powrot do MENU";
        napisRanking.setString(ss.str());
    }
    case EtapGry::ROZGRYWKA:
    {
        // RUCH POSTACI (blokowanie) => wywolujemy dopiero tutaj
        if (postac)
        {
            // klawisze
            sf::Vector2f d(0.f, 0.f);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  d.x = -1.f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) d.x = 1.f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    d.y = -1.f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  d.y = 1.f;
            postac->ustawKierunek(d);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))    punkty += 1;

            // blokowanie
            auto sciany = pobierzScianyAktPoziomu();
            postac->probujRuch(dt, sciany);

            postac->aktualizuj(dt); // ewentualnie
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
 
        }

        auto& p = poziomy[indeksPoziomu];

        for (auto d : p.duchy)
            d->aktualizuj(dt);

        // kolizja z duchem => +1
        if (postac)
        {
            for (auto w : p.duchy)
            {
                if (postac->pobierzGranice().intersects(w->pobierzGranice()))
                {
                    postac->dodajKolizjeZduchem();
                    postac->ustawPozycje(SZEROKOSC_OKNA / 3.f, WYSOKOSC_OKNA / 3.f);
                }
            }
            if (postac->czyPrzegrana())
            {
                etap = EtapGry::GAME_OVER;
                break;
            }
        }

        // zbieranie jablek => +30
        for (auto j : p.jablka)
        {
            if (!j->czyZebrane() && postac->pobierzGranice().intersects(j->pobierzGranice()))
            {
                j->oznaczZebrane();
                punkty += 30;
            }
        }

        // 150 -> next
        if (punkty >= 150)
        {
            indeksPoziomu++;
            if (indeksPoziomu >= (int)poziomy.size())
            {
                indeksPoziomu = (int)poziomy.size() - 1;
                etap = EtapGry::MENU;
                indeksPoziomu = 0;
            }
            else
            {
                punkty -= 150;
                postac->ustawPozycje(SZEROKOSC_OKNA / 2.f, WYSOKOSC_OKNA / 2.f);
            }
        }

        std::ostringstream ss;
        ss << "Poziom: " << (indeksPoziomu + 1)
            << "  |  Punkty: " << punkty << " / 150"
            << "  |  Zycia: " << 3 - (postac->ileKolizjiZduchem()) << " / 3";
        napisStatus.setString(ss.str());
    }
    break;

    case EtapGry::POMOC:
        break;

    case EtapGry::WYJSCIE_POTWIERDZENIE:
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
            etap = EtapGry::KONIEC;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
            etap = EtapGry::ROZGRYWKA;
    }
    break;

    case EtapGry::GAME_OVER:
        break;

    default:
        break;
    }
}

void SilnikGry::rysuj()
{
    okno.clear(sf::Color::Black);

    switch (etap)
    {
    case EtapGry::MENU:
        okno.draw(tlo);
        okno.draw(napisStatus);
        break;

    case EtapGry::NICK:
        okno.draw(napisNick);
        break;

    case EtapGry::ROZGRYWKA:
    {
        okno.draw(tlo);
        auto& lvl = poziomy[indeksPoziomu];

        // sciany
        for (auto n : lvl.sciankiNiereg)
            n->rysuj(okno);
        for (auto pr : lvl.sciankiProst)
            pr->rysuj(okno);
        // jablka
        for (auto jb : lvl.jablka)
            jb->rysuj(okno);
        // duchy
        for (auto d : lvl.duchy)
            d->rysuj(okno);

        if (postac) postac->rysuj(okno);

        okno.draw(napisStatus);
    }
    break;

    case EtapGry::RANKING:
    {
        okno.draw(tlo);
        sf::Text txt("Ranking:", czcionka, 40);
        txt.setFillColor(sf::Color::Red);
        txt.setOutlineColor(sf::Color::Black);
        txt.setOutlineThickness(2.f);
        txt.setPosition(400.f, 300.f);
        okno.draw(txt);
    }
    break;

    case EtapGry::POMOC:
        okno.draw(tlo);
        okno.draw(napisPomoc);
        break;

    case EtapGry::WYJSCIE_POTWIERDZENIE:
    {
        okno.draw(tlo);
        sf::Text txt("Wyjsc z programu? [T/N]", czcionka, 40);
        txt.setFillColor(sf::Color::Red);
        txt.setOutlineColor(sf::Color::Black);
        txt.setOutlineThickness(2.f);
        txt.setPosition(400.f, 300.f);
        okno.draw(txt);
    }
    break;

    case EtapGry::GAME_OVER:
        okno.draw(tlo);
        okno.draw(napisGameOver);
        break;

    default:
        break;
    }

    okno.display();
}
