#ifndef SILNIK_H
#define SILNIK_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Obiekty.h"
#include "Zasoby.h"
#include "Ranking.h"

enum class EtapGry
{
    MENU,
    NICK,
    ROZGRYWKA,
    RANKING,
    POMOC,
    WYJSCIE_POTWIERDZENIE,
    KONIEC,
    GAME_OVER,
    WYGRANA
};  

struct Poziom
{
    std::vector<Przeciwnik*> duchy;
    std::vector<PrzeszkodaNieregularna*> sciankiNiereg;
    std::vector<PrzeszkodaProstokatna*> sciankiProst;
    std::vector<Jablko*> jablka;
};

class SilnikGry
{
private:
    sf::RenderWindow okno;
    EtapGry etap;
    ZasobyGry zasoby;
    std::string aktualnyNick;
    Postac* postac;
    std::vector<Poziom> poziomy;
    bool nicknameEntered;

    int indeksPoziomu;
    int punkty;

    sf::Sprite tlo;
    sf::Font czcionka;
    sf::Text napisStatus;
    sf::Text napisPomoc;
    sf::Text napisGameOver;
    sf::Text napisRanking;
    sf::Text napisNick;

    std::vector<GraczRanking> gracze;
    sf::Clock zegar;

public:
    SilnikGry();
    ~SilnikGry();
    void uruchom();

private:
    void inicjujPoziomy();

    void petlaZdarzen();
    void aktualizuj(float dt);
    void rysuj();

    void zapytajNick(const sf::Event& event);
    // Zbiera liste floatRect wszystkich Scian (prostok i niereg)
    std::vector<sf::FloatRect> pobierzScianyAktPoziomu() const;
};


#endif // SILNIK_H
