//
// Created by Amadeus Gebauer on 07.05.18.
//

#include <gtest/gtest.h>
#include <fstream>
#include "../src/LINALG/Vector.h"
#include "../src/NUMPY/NumpyUtils.h"
#include "../src/NUMPY/NumpyIOException.h"

TEST(NumpyTests, ReadHeaderTest) {

    // open file
    std::ifstream myFile ("test/data/testarr.npy", std::ios::in | std::ios::binary);

    std::vector<unsigned long> shape(0);
    NUMPY::NumpyUtils::readHeader(myFile, shape);

    myFile.seekg(0);
    ASSERT_THROW(

    // open file
            std::ifstream myFileFail ("test/NumpyIOTest.cpp", std::ios::in | std::ios::binary);
            NUMPY::NumpyUtils::readHeader(myFileFail, shape),
            NUMPY::NumpyIOException
    );
}

TEST(NumpyTests, FullVectorFailTest) {
    LINALG::Vector vector(0);
    ASSERT_THROW(
            NUMPY::NumpyUtils::readFullVector("test/data/testmat.npy", vector);
    , NUMPY::NumpyIOException
    );
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

TEST(NumpyTests, FullMatrixFailTest) {
    LINALG::SymmetricMatrix matrix(0);
    ASSERT_THROW(
            NUMPY::NumpyUtils::readFullSymmetricMatrix("test/data/testarr.npy", matrix);
    , NUMPY::NumpyIOException
    );
}

TEST(NumpyTests, WriteDistributedVectorTest) {
    unsigned long size = 1000, startRow, localSize;

    MPI::MpiInfo info = MPI::MpiInfo::Create();
    info.getLocalProblemDims(size, startRow, localSize);


    LINALG::DistributedVector vector(size, startRow, localSize);

    for(unsigned long i=startRow;i<startRow+localSize;++i) {
        vector(i) = i;
    }

    NUMPY::NumpyUtils::writeDistributedVector("test/data/testarroutparallel.npy", vector, info);
}