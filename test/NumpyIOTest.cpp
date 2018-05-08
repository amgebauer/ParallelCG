//
// Created by Amadeus Gebauer on 07.05.18.
//

#include <gtest/gtest.h>
#include <fstream>
#include "../src/LINALG/Vector.h"
#include "../src/NUMPY/NumpyUtils.h"

TEST(NumpyTests, ReadHeaderTest) {

    // open file
    std::ifstream myFile ("test/data/testarr.npy", std::ios::in | std::ios::binary);

    std::vector<unsigned long> shape(0);
    NUMPY::NumpyUtils::readHeader(myFile, shape);
}

TEST(NumpyTests, ReadFullVectorTest) {
    LINALG::Vector vector(0);


    NUMPY::NumpyUtils::readFullVector("test/data/testarr.npy", vector);

    ASSERT_EQ(1000, vector.getSize());

    for(int i=0;i<1000;i++) {
        ASSERT_FLOAT_EQ(i, vector(i));
    }

}

TEST(NumpyTests, WriteFullVectorTest) {
    LINALG::Vector vector(1000);

    for(unsigned long i=0;i<1000;++i) {
        vector(i) = i;
    }


    NUMPY::NumpyUtils::writeFullVector("test/data/testarrout.npy", vector);
}

TEST(NumpyTests, ReadFullMatrixTest) {
    LINALG::SymmetricMatrix matrix(0);


    NUMPY::NumpyUtils::readFullSymmetricMatrix("test/data/testmat.npy", matrix);

    ASSERT_EQ(100, matrix.getSize());

    for(unsigned long i=0;i<100;i++) {
        for (unsigned long j=i;j<100;++j) {
            ASSERT_FLOAT_EQ(i+j, matrix(i, j));
        }
    }

}