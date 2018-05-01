//
// Created by Amadeus Gebauer on 01.05.18.
//

#include <gtest/gtest.h>
#include "../src/Utils/Random.h"

TEST(RandomTests, DoubleTest) {

    const int numberOfRandoms = 100000;
    double mean = 0.0;
    for (int i=0;i<numberOfRandoms;++i) {
        double rnd = UTIL::Random::getRandom(-100, 100);

        mean += rnd;

        ASSERT_LE(rnd, 100);
        ASSERT_GE(rnd, -100);
    }

    ASSERT_NEAR(0, mean/numberOfRandoms, 1);
}