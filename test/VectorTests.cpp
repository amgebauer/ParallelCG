//
// Created by Amadeus Gebauer on 30.04.18.
//

#include <gtest/gtest.h>
#include <cmath>
#include "../src/LINALG/Vector.h"
#include "../src/Utils/Random.h"
#include "../src/LINALG/DimensionMismatch.h"

TEST(VectorTests, ScalarProduct) {

    const int vectorLength = 100;
    // generate two random vectors
    double correctResult = 0.0;
    LINALG::Vector v1(vectorLength);
    LINALG::Vector v2(vectorLength);

    for (int i=0;i<vectorLength;i++) {
        double randomNumber1 = UTIL::Random::getRandom(-100, 100);
        double randomNumber2 = UTIL::Random::getRandom(-100, 100);
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
        double randomNumber1 = UTIL::Random::getRandom(-100, 100);
        double randomNumber2 = UTIL::Random::getRandom(-100, 100);
        v1(i) = randomNumber1;
        v2(i) = randomNumber2;
        correctResult += pow(randomNumber1+randomNumber2, 2);
    }

    ASSERT_FLOAT_EQ(correctResult, (v1+v2)*(v1+v2));
}

TEST(VectorTests, InPlaceVectorAdd) {

    const int vectorLength = 100;
    // generate two random vectors
    double correctResult = 0.0;
    LINALG::Vector v1(vectorLength);
    LINALG::Vector v2(vectorLength);

    for (int i=0;i<vectorLength;i++) {
        double randomNumber1 = UTIL::Random::getRandom(-100, 100);
        double randomNumber2 = UTIL::Random::getRandom(-100, 100);
        v1(i) = randomNumber1;
        v2(i) = randomNumber2;
        correctResult += pow(randomNumber1+randomNumber2, 2);
    }

    v1.add(v2);

    ASSERT_FLOAT_EQ(correctResult, v1.normSquared());
}

TEST(VectorTests, InPlaceVectorSub) {

    const int vectorLength = 100;
    // generate two random vectors
    double correctResult = 0.0;
    LINALG::Vector v1(vectorLength);
    LINALG::Vector v2(vectorLength);

    for (int i=0;i<vectorLength;i++) {
        double randomNumber1 = UTIL::Random::getRandom(-100, 100);
        double randomNumber2 = UTIL::Random::getRandom(-100, 100);
        v1(i) = randomNumber1;
        v2(i) = randomNumber2;
        correctResult += pow(randomNumber1-randomNumber2, 2);
    }

    v1.sub(v2);

    ASSERT_FLOAT_EQ(correctResult, v1.normSquared());
}

TEST(VectorTests, InPlaceScale) {

    double scaleFactor = 4324.31;
    const int vectorLength = 100;
    // generate two random vectors
    double correctResult = 0.0;
    LINALG::Vector vector(vectorLength);

    for (int i=0;i<vectorLength;i++) {
        double randomNumber = UTIL::Random::getRandom(-100, 100);
        vector(i) = randomNumber;
        correctResult += pow(randomNumber*scaleFactor, 2);
    }

    LINALG::Vector unscaledVector(vector);
    vector.scale(scaleFactor);

    for(int i=0;i<vector.getSize();++i) {
        ASSERT_FLOAT_EQ(vector(i), unscaledVector(i)*scaleFactor);
    }

    ASSERT_FLOAT_EQ(correctResult, vector.normSquared());
}
TEST(VectorTests, DimensionException) {
    LINALG::Vector vector1(100);
    LINALG::Vector vector2(101);

    ASSERT_THROW(vector1+vector2, LINALG::DimensionMismatch);
}