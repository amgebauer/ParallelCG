//
// Created by Amadeus Gebauer on 30.04.18.
//

#include <gtest/gtest.h>
#include "../src/LINALG/Vector.h"
#include "../src/LINALG/DistributedVector.h"

TEST(DistributedVectorTests, ScalarProduct) {

    const int localSize = 100;
    MPI::MpiInfo mpiInfo = MPI::MpiInfo::Create();
    const int vectorLength = localSize*mpiInfo.getSize();


    // generate two random vectors
    double correctResult = 0.0;
    LINALG::DistributedVector v1((unsigned long) vectorLength,
                                 static_cast<unsigned long>(localSize * mpiInfo.getRank()), localSize);
    LINALG::DistributedVector v2((unsigned long) vectorLength,
                                 static_cast<unsigned long>(localSize * mpiInfo.getRank()), localSize);

    for (int i=0;i<localSize;++i) {
        v1(static_cast<unsigned long>(mpiInfo.getRank() * localSize + i)) = mpiInfo.getRank()*localSize+i;
        v2(static_cast<unsigned long>(mpiInfo.getRank() * localSize + i)) = -(mpiInfo.getRank()*localSize+i);
    }

    for (int i=0;i<vectorLength;++i) {
        correctResult += i*(-i);
    }

    ASSERT_FLOAT_EQ(correctResult, v1.distributedProduct(v2, mpiInfo));
}

TEST(DistributedVectorTests, GetFull) {

    // test for different local sizes

    for (int localSize=100;localSize<150;++localSize) {
        MPI::MpiInfo mpiInfo = MPI::MpiInfo::Create();
        const int vectorLength = localSize*mpiInfo.getSize()+1;

        unsigned long myStartRow = static_cast<unsigned long>(localSize * mpiInfo.getRank());
        unsigned long mySize = localSize;
        if(mpiInfo.getRank() == 0) {
            mySize += 1;
        } else {
            myStartRow += 1;
        }

        // generate two random vectors
        double correctResult = 0.0;
        LINALG::DistributedVector vector((unsigned long) vectorLength, myStartRow, mySize);

        for (int i=0;i<mySize;++i) {
            vector(myStartRow+i) = myStartRow+i;
        }

        LINALG::Vector fullVector = vector.getFull(mpiInfo);

        if(mpiInfo.getRank() == 0) {
            for (int i=0;i<vectorLength;++i) {
            }
        }

        ASSERT_GT(vectorLength, 0);
        for (int i=0;i<vectorLength;++i) {
            ASSERT_FLOAT_EQ(fullVector(i), i);
        }
    }

}