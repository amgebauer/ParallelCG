//
// Created by Amadeus Gebauer on 01.05.18.
//

#include <cstdlib>
#include "Random.h"

double UTIL::Random::getRandom(double from, double to) {
    return ((double)rand() / RAND_MAX)*(to-from)+from;
}
