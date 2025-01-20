#ifndef ZASOBY_H
#define ZASOBY_H

#include <SFML/Graphics.hpp>

class ZasobyGry
{
private:
    sf::Texture texTloLabirynt;
    sf::Texture texPostac;
    sf::Texture texPrzeciwnik;
    sf::Texture texJablko;

public:
    ZasobyGry();
    const sf::Texture& wezTloLabirynt() const;
    const sf::Texture& wezTexPostac() const;
    const sf::Texture& wezTexPrzeciwnik() const;
    const sf::Texture& wezTexJablko() const;
};

#endif // ZASOBY_H
