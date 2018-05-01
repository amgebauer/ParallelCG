//
// Created by Amadeus Gebauer on 01.05.18.
//

#include <gtest/gtest.h>
#include "../src/LINALG/SymmetricMatrix.h"
#include "../src/Utils/Random.h"

TEST(SymmetricMatrixTests, vectorSquaredMultiplication) {

    // generate positive definite matrix
    const int size = 100;

    LINALG::SymmetricMatrix matrix(size);
    LINALG::Vector vector(size);

    for (unsigned long i=0;i<matrix.getSize();++i) {

        for (unsigned long j=i;j<matrix.getSize();++j) {
            matrix(i,j) = UTIL::Random::getRandom(-10, 10);
            if(i == j) matrix(i,j) += UTIL::Random::getRandom(size+10, size+100);
        }
        vector(i) = UTIL::Random::getRandom(-10, 10);
    }


    ASSERT_FLOAT_EQ(vector*(matrix*vector), matrix.vectorSquaredMul(vector));
}