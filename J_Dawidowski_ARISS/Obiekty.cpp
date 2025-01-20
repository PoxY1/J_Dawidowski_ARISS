#include "Obiekty.h"
#include <cmath>

// -----------------------------
// POSTAC
// -----------------------------
Postac::Postac(const sf::Texture& tex)
{
    spritePostaci.setTexture(tex);
    spritePostaci.setScale(24.f / tex.getSize().x, 32.f / tex.getSize().y);
    spritePostaci.setOrigin(tex.getSize().x / 2.f, tex.getSize().y / 2.f);
    spritePostaci.setPosition(SZEROKOSC_OKNA / 2.f, WYSOKOSC_OKNA / 2.f);
    szybkosc = 200.f;
    kierunekRuchu = { 0.f, 0.f };
    kolizjeZduchem = 0;
}

void Postac::aktualizuj(float)
{
    // Pusta – bo docelowo ruch jest w probujRuch()
}

void Postac::rysuj(sf::RenderWindow& okno)
{
    okno.draw(spritePostaci);
}

sf::FloatRect Postac::pobierzGranice() const
{
    return spritePostaci.getGlobalBounds();
}

void Postac::ustawPozycje(float x, float y)
{
    spritePostaci.setPosition(x, y);
}

void Postac::ustawKierunek(const sf::Vector2f& kier)
{
    kierunekRuchu = kier;
}

bool Postac::czyPrzegrana() const
{
    return (kolizjeZduchem >= 3);
}

void Postac::dodajKolizjeZduchem()
{
    kolizjeZduchem++;
}
void Postac::resetujKolizjeZduchem()
{
    kolizjeZduchem = 0;
}
int Postac::ileKolizjiZduchem()
{
    return kolizjeZduchem;
}
void Postac::ustawNick(std::string pseudonim)
{
    nick = pseudonim;
}
std::string Postac::sprawdzNick()
{
    return nick;
}
void Postac::ustawWynik(int score)
{
    wynik = score;
}
// Glowna logika chodzenia + blokowania
void Postac::probujRuch(float delta, const std::vector<sf::FloatRect>& sciany)
{
    sf::Vector2f pos = spritePostaci.getPosition();
    sf::Vector2f ruch = kierunekRuchu * szybkosc * delta;

    sf::Vector2f nowaPos = pos + ruch;

    // Ograniczenia ekranu
    if (nowaPos.x < 0.f) nowaPos.x = 0.f;
    if (nowaPos.x > SZEROKOSC_OKNA) nowaPos.x = (float)SZEROKOSC_OKNA;
    if (nowaPos.y < 0.f) nowaPos.y = 0.f;
    if (nowaPos.y > WYSOKOSC_OKNA) nowaPos.y = (float)WYSOKOSC_OKNA;

    spritePostaci.setPosition(nowaPos);

    // Sprawdzamy kolizje z kazda sciana
    for (auto& s : sciany)
    {
        if (spritePostaci.getGlobalBounds().intersects(s))
        {
            // Cofamy do starej
            spritePostaci.setPosition(pos);
            return;
        }
    }
}

// -----------------------------
// PRZECIWNIK
// -----------------------------
Przeciwnik::Przeciwnik(const sf::Texture& tex)
{
    spriteWroga.setTexture(tex);
    spriteWroga.setScale(64.f / tex.getSize().x, 48.f / tex.getSize().y);
    spriteWroga.setOrigin(tex.getSize().x / 2.f, tex.getSize().y / 2.f);
    spriteWroga.setPosition(300.f, 200.f);
    predkosc = 100.f;
    wektorRuchu = { 0.f, 1.f };
}

void Przeciwnik::aktualizuj(float dt)
{
    sf::Vector2f p = spriteWroga.getPosition();
    p += wektorRuchu * predkosc * dt;

    // Odbicia
    if (p.y < 100.f) wektorRuchu.y = std::abs(wektorRuchu.y);
    if (p.y > WYSOKOSC_OKNA - 100.f) wektorRuchu.y = -std::abs(wektorRuchu.y);

    spriteWroga.setPosition(p);
}

void Przeciwnik::rysuj(sf::RenderWindow& okno)
{
    okno.draw(spriteWroga);
}

sf::FloatRect Przeciwnik::pobierzGranice() const
{
    return spriteWroga.getGlobalBounds();
}

void Przeciwnik::ustawPozycje(float x, float y)
{
    spriteWroga.setPosition(x, y);
}

// -----------------------------
// PRZESZKODA NIEREGULARNA
// -----------------------------
PrzeszkodaNieregularna::PrzeszkodaNieregularna(sf::Vector2f pos, sf::Color color)
{
    sf::ConvexShape shape;
    shape.setPointCount(5);
    shape.setPoint(0, { 0.f, 0.f });
    shape.setPoint(1, { 50.f, 0.f });
    shape.setPoint(2, { 70.f, 30.f });
    shape.setPoint(3, { 40.f, 60.f });
    shape.setPoint(4, { 0.f, 40.f });

    shape.setFillColor(color);
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(2.f);

    shape.setPosition(pos);
    ksztalt = shape;
}

void PrzeszkodaNieregularna::rysuj(sf::RenderWindow& okno)
{
    okno.draw(ksztalt);
}

sf::FloatRect PrzeszkodaNieregularna::pobierzGranice() const
{
    return ksztalt.getGlobalBounds();
}

// -----------------------------
// PRZESZKODA PROSTOKATNA
// -----------------------------
PrzeszkodaProstokatna::PrzeszkodaProstokatna(sf::Vector2f pos, sf::Vector2f size, sf::Color color)
{
    sf::RectangleShape rect(size);
    rect.setFillColor(color);
    rect.setOutlineThickness(0.f);
    rect.setOutlineColor(sf::Color::Black);
    rect.setPosition(pos);

    prostokat = rect;
}

void PrzeszkodaProstokatna::rysuj(sf::RenderWindow& okno)
{
    okno.draw(prostokat);
}

sf::FloatRect PrzeszkodaProstokatna::pobierzGranice() const
{
    return prostokat.getGlobalBounds();
}

// -----------------------------
// JABLKO (+30)
// -----------------------------
Jablko::Jablko(const sf::Texture& tex, sf::Vector2f pos)
{
    spriteJablko.setTexture(tex);
    spriteJablko.setScale(32.f / tex.getSize().x, 32.f / tex.getSize().y);
    spriteJablko.setOrigin(tex.getSize().x / 2.f, tex.getSize().y / 2.f);
    spriteJablko.setPosition(pos);
    zebrane = false;
}

void Jablko::rysuj(sf::RenderWindow& okno)
{
    if (!zebrane)
        okno.draw(spriteJablko);
}

sf::FloatRect Jablko::pobierzGranice() const
{
    return spriteJablko.getGlobalBounds();
}

bool Jablko::czyZebrane() const
{
    return zebrane;
}

void Jablko::oznaczZebrane()
{
    zebrane = true;
}
void Jablko::resetujZebranie()
{
    zebrane = false;
}
