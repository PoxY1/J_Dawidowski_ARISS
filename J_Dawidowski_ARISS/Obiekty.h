#ifndef OBIEKTY_H
#define OBIEKTY_H

#include <SFML/Graphics.hpp>
#include <string>

static const unsigned SZEROKOSC_OKNA = 1280;
static const unsigned WYSOKOSC_OKNA = 720;

class ElementGry
{
public:
    virtual ~ElementGry() {}
    virtual void aktualizuj(float delta) = 0;
    virtual void rysuj(sf::RenderWindow& okno) = 0;
};

class Postac : public ElementGry
{
private:
    sf::Sprite spritePostaci;
    sf::Vector2f kierunekRuchu;
    float szybkosc;
    int kolizjeZduchem;
    std::string nick;
    int wynik;

public:
    Postac(const sf::Texture& tex);
    void aktualizuj(float delta) override;  
    void rysuj(sf::RenderWindow& okno) override;

    sf::FloatRect pobierzGranice() const;
    void ustawPozycje(float x, float y);
    void ustawKierunek(const sf::Vector2f& kier);

    bool czyPrzegrana() const;
    void dodajKolizjeZduchem();
    void resetujKolizjeZduchem();
    int ileKolizjiZduchem();
    void ustawNick(std::string pseudonim);
    std::string sprawdzNick();
    void ustawWynik(int score);


    // blokowanie
    void probujRuch(float delta, const std::vector<sf::FloatRect>& sciany);
};

class Przeciwnik : public ElementGry
{
private:
    sf::Sprite spriteWroga;
    sf::Vector2f wektorRuchu;
    float predkosc;
public:
    Przeciwnik(const sf::Texture& tex);
    void aktualizuj(float delta) override;
    void rysuj(sf::RenderWindow& okno) override;

    sf::FloatRect pobierzGranice() const;
    void ustawPozycje(float x, float y);
};

class PrzeszkodaNieregularna : public ElementGry
{
private:
    sf::ConvexShape ksztalt;
public:
    PrzeszkodaNieregularna(sf::Vector2f pos, sf::Color color);
    void aktualizuj(float) override {}
    void rysuj(sf::RenderWindow& okno) override;
    sf::FloatRect pobierzGranice() const;
};

class PrzeszkodaProstokatna : public ElementGry
{
private:
    sf::RectangleShape prostokat;
public:
    PrzeszkodaProstokatna(sf::Vector2f pos, sf::Vector2f size, sf::Color color);
    void aktualizuj(float) override {}
    void rysuj(sf::RenderWindow& okno) override;
    sf::FloatRect pobierzGranice() const;
};

class Jablko : public ElementGry
{
private:
    sf::Sprite spriteJablko;
    bool zebrane;
public:
    Jablko(const sf::Texture& tex, sf::Vector2f pos);
    void aktualizuj(float) override {}
    void rysuj(sf::RenderWindow& okno) override;

    sf::FloatRect pobierzGranice() const;
    bool czyZebrane() const;
    void oznaczZebrane();
    void resetujZebranie();
};

#endif // OBIEKTY_H
