//
// Created by Amadeus Gebauer on 01.05.18.
//


#include <gtest/gtest.h>
#include "../src/LINALG/SymmetricMatrix.h"
#include "../src/LINALG/DistributedSymmetricMatrix.h"
#include "../src/Utils/Random.h"

TEST(DistributedSymmetricMatrixTest, MatrixVectorMul) {
    const int size = 100;

    LINALG::SymmetricMatrix fullMatrix(size);
    LINALG::DistributedSymmetricMatrix distributedMatrix(size, 0, size);

    LINALG::Vector fullVector(size);

    for(unsigned long i=0;i<size;++i) {

        for(unsigned long j=i;j<size;j++) {
            double rnd = UTIL::Random::getRandom(-100, 100);
            fullMatrix(i,j) = rnd;
            distributedMatrix(i,j) = rnd;

        }
        double rnd = UTIL::Random::getRandom(-100, 100);
        fullVector(i) = rnd;

    }

    LINALG::Vector referenceSolution = fullMatrix*fullVector;
    LINALG::DistributedVector distributedSolution = distributedMatrix*fullVector;

    // compare both
    for(unsigned long i=0;i<size;++i) {

        ASSERT_FLOAT_EQ(referenceSolution(i), distributedSolution(i));

    }
}