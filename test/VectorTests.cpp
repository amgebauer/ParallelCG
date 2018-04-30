//
// Created by Amadeus Gebauer on 30.04.18.
//

#include <gtest/gtest.h>
#include <cmath>
#include "../src/LINALG/Vector.h"

TEST(VectorTests, ScalarProduct) {

    const int vectorLength = 100;
    // generate two random vectors
    double correctResult = 0.0;
    LINALG::Vector v1(vectorLength);
    LINALG::Vector v2(vectorLength);

    for (int i=0;i<vectorLength;i++) {
        double randomNumber1 = 100*((double)rand() / RAND_MAX-0.5);
        double randomNumber2 = 100*((double)rand() / RAND_MAX-0.5);
        v1(i) = randomNumber1;
        v2(i) = randomNumber2;
        correctResult += randomNumber1*randomNumber2;
    }

    ASSERT_FLOAT_EQ(correctResult, v1*v2);
}

TEST(VectorTests, VectorAdd) {

    const int vectorLength = 100;
    // generate two random vectors
    double correctResult = 0.0;
    LINALG::Vector v1(vectorLength);
    LINALG::Vector v2(vectorLength);

    for (int i=0;i<vectorLength;i++) {
        double randomNumber1 = 100*((double)rand() / RAND_MAX-0.5);
        double randomNumber2 = 100*((double)rand() / RAND_MAX-0.5);
        v1(i) = randomNumber1;
        v2(i) = randomNumber2;
        correctResult += pow(randomNumber1+randomNumber2, 2);
    }

    ASSERT_FLOAT_EQ(correctResult, (v1+v2)*(v1+v2));
}