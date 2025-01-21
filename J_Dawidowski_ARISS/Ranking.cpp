#include "Ranking.h"
#include <fstream>
#include <iostream>
#include <algorithm>

bool zapiszStan(const std::vector<GraczRanking>& dane)
{
    std::ofstream plik("dane.txt");
    if (!plik.is_open())
    {
        std::cerr << "Nie mozna otworzyc dane.txt do zapisu.\n";
        return false;
    }

   plik << dane.size() << "\n";

    for (auto& gr : dane)
    {
        plik << gr.nick << " " << gr.punkty << "\n";
    }
    plik.close();
    return true;
}



bool wczytajStan(std::vector<GraczRanking>& dane)
{
    std::ifstream plik("dane.txt");
    if (!plik.is_open())
    {
        std::cerr << "Brak pliku dane.txt lub nie udalo sie otworzyc.\n";
        return false;
    }

    size_t ile;
    plik >> ile;
    dane.clear();
    dane.reserve(ile);

    for (size_t i = 0; i < ile; i++)
    {
        GraczRanking gr;
        plik >> gr.nick >> gr.punkty;
        dane.push_back(gr);
    }
    plik.close();

    std::sort(dane.begin(), dane.end(),
        [](const GraczRanking& a, const GraczRanking& b) {
            return a.punkty > b.punkty;
        }
    );
    return true;
}
