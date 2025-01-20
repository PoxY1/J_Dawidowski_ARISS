#include "Zasoby.h"
#include <iostream>

ZasobyGry::ZasobyGry()
{
    if (!texTloLabirynt.loadFromFile("assets/tloLabirynt.png"))
        std::cerr << "Blad wczytania assets/tloLabirynt.png\n";

    if (!texPostac.loadFromFile("assets/postac.png"))
        std::cerr << "Blad wczytania assets/postac.png\n";

    if (!texPrzeciwnik.loadFromFile("assets/przeciwnik.png"))
        std::cerr << "Blad wczytania assets/przeciwnik.png\n";

    if (!texJablko.loadFromFile("assets/apple.png"))
        std::cerr << "Blad wczytania assets/apple.png\n";
}

const sf::Texture& ZasobyGry::wezTloLabirynt() const
{
    return texTloLabirynt;
}
const sf::Texture& ZasobyGry::wezTexPostac() const
{
    return texPostac;
}
const sf::Texture& ZasobyGry::wezTexPrzeciwnik() const
{
    return texPrzeciwnik;
}
const sf::Texture& ZasobyGry::wezTexJablko() const
{
    return texJablko;
}
