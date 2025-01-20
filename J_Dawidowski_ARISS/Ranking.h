#ifndef RANKING_H
#define RANKING_H

#include "Obiekty.h"
#include <vector>
#include <string>

struct GraczRanking
{
    std::string nick;
    int punkty;
};

bool zapiszStan(const std::vector<GraczRanking>& dane);
bool wczytajStan(std::vector<GraczRanking>& dane);

#endif // RANKING_H
