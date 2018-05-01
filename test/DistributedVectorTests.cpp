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

    ASSERT_FLOAT_EQ(correctResult, v1.distributedProduct(v2));
}

TEST(DistributedVectorTests, NormSquared) {

    const int localSize = 100;
    MPI::MpiInfo mpiInfo = MPI::MpiInfo::Create();
    const int vectorLength = localSize*mpiInfo.getSize();


    // generate two random vectors
    double correctResult = 0.0;
    LINALG::DistributedVector v1((unsigned long) vectorLength,
                                 static_cast<unsigned long>(localSize * mpiInfo.getRank()), localSize);

    for (int i=0;i<localSize;++i) {
        v1(static_cast<unsigned long>(mpiInfo.getRank() * localSize + i)) = mpiInfo.getRank()*localSize+i;
    }

    for (int i=0;i<vectorLength;++i) {
        correctResult += i*i;
    }

    ASSERT_FLOAT_EQ(correctResult, v1.distributedNormSquared());
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

        // generate vectors
        LINALG::DistributedVector vector((unsigned long) vectorLength, myStartRow, mySize);

        for (unsigned long i=0;i<mySize;++i) {
            vector(myStartRow+i) = myStartRow+i;
        }

        LINALG::Vector fullVector = vector.getFull(mpiInfo);

        ASSERT_GT(vectorLength, 0);
        for (int i=0;i<vectorLength;++i) {
            ASSERT_FLOAT_EQ(fullVector(i), i);
        }
    }

}

TEST(DistributedVectorTests, VectorAdd) {

    const int localSize = 100;
    MPI::MpiInfo mpiInfo = MPI::MpiInfo::Create();
    const int vectorLength = localSize*mpiInfo.getSize();


    // generate two random vectors
    LINALG::DistributedVector v1((unsigned long) vectorLength,
                                 static_cast<unsigned long>(localSize * mpiInfo.getRank()), localSize);
    LINALG::DistributedVector v2((unsigned long) vectorLength,
                                 static_cast<unsigned long>(localSize * mpiInfo.getRank()), localSize);

    for (int i=0;i<localSize;++i) {
        v1(static_cast<unsigned long>(mpiInfo.getRank() * localSize + i)) = mpiInfo.getRank()*localSize+i;
        v2(static_cast<unsigned long>(mpiInfo.getRank() * localSize + i)) = -(mpiInfo.getRank()*localSize+i);
    }

    LINALG::DistributedVector v3 = v1+v2;

    ASSERT_GT(v2.getLocalSize(), 1);

    for(unsigned long i=v3.getStartRow();i<v3.getStartRow()+v3.getLocalSize();++i) {
        ASSERT_FLOAT_EQ(v3(i), 0);
    }
}

TEST(DistributedVectorTests, VectorSub) {

    const int localSize = 100;
    MPI::MpiInfo mpiInfo = MPI::MpiInfo::Create();
    const int vectorLength = localSize*mpiInfo.getSize();


    // generate two random vectors
    LINALG::DistributedVector v1((unsigned long) vectorLength,
                                 static_cast<unsigned long>(localSize * mpiInfo.getRank()), localSize);
    LINALG::DistributedVector v2((unsigned long) vectorLength,
                                 static_cast<unsigned long>(localSize * mpiInfo.getRank()), localSize);

    for (int i=0;i<localSize;++i) {
        v1(static_cast<unsigned long>(mpiInfo.getRank() * localSize + i)) = mpiInfo.getRank()*localSize+i;
        v2(static_cast<unsigned long>(mpiInfo.getRank() * localSize + i)) = -(mpiInfo.getRank()*localSize+i);
    }

    LINALG::DistributedVector v3 = v1-v2;

    ASSERT_GT(v2.getLocalSize(), 1);

    for(unsigned long i=v3.getStartRow();i<v3.getStartRow()+v3.getLocalSize();++i) {
        ASSERT_FLOAT_EQ(v3(i), 2*i);
    }
}

TEST(DistributedVectorTests, OutOfRange) {

    const int localSize = 100;
    MPI::MpiInfo mpiInfo = MPI::MpiInfo::Create();
    const int vectorLength = localSize*mpiInfo.getSize();


    // generate two random vectors
    LINALG::DistributedVector v1((unsigned long) vectorLength,
                                 static_cast<unsigned long>(localSize * mpiInfo.getRank()), localSize);

    ASSERT_THROW(
        for(unsigned long i=0;i<v1.getSize();++i) {
            v1(i) = 1;
        }
    , std::out_of_range);

    ASSERT_THROW(
            for(unsigned long i=0;i<v1.getSize();++i) {
                v1.get(i);
            }
    , std::out_of_range);

}

TEST(DistributedVectorTests, InPlaceVectorAdd) {

    const int localSize = 100;
    MPI::MpiInfo mpiInfo = MPI::MpiInfo::Create();
    const int vectorLength = localSize*mpiInfo.getSize();


    // generate two random vectors
    LINALG::DistributedVector v1((unsigned long) vectorLength,
                                 static_cast<unsigned long>(localSize * mpiInfo.getRank()), localSize);
    LINALG::DistributedVector v2((unsigned long) vectorLength,
                                 static_cast<unsigned long>(localSize * mpiInfo.getRank()), localSize);

    for (int i=0;i<localSize;++i) {
        v1(static_cast<unsigned long>(mpiInfo.getRank() * localSize + i)) = mpiInfo.getRank()*localSize+i;
        v2(static_cast<unsigned long>(mpiInfo.getRank() * localSize + i)) = -(mpiInfo.getRank()*localSize+i);
    }

    v1.add(v2);

    ASSERT_GT(v2.getLocalSize(), 1);

    for(unsigned long i=v1.getStartRow();i<v1.getStartRow()+v1.getLocalSize();++i) {
        ASSERT_FLOAT_EQ(v1(i), 0);
    }
}

TEST(DistributedVectorTests, InPlaceVectorAddMul) {

    const int localSize = 100;
    double scaleOther = 432.2;
    MPI::MpiInfo mpiInfo = MPI::MpiInfo::Create();
    const int vectorLength = localSize*mpiInfo.getSize();


    // generate two random vectors
    LINALG::DistributedVector v1((unsigned long) vectorLength,
                                 static_cast<unsigned long>(localSize * mpiInfo.getRank()), localSize);
    LINALG::DistributedVector v2((unsigned long) vectorLength,
                                 static_cast<unsigned long>(localSize * mpiInfo.getRank()), localSize);

    for (int i=0;i<localSize;++i) {
        v1(static_cast<unsigned long>(mpiInfo.getRank() * localSize + i)) = mpiInfo.getRank()*localSize+i;
        v2(static_cast<unsigned long>(mpiInfo.getRank() * localSize + i)) = -(mpiInfo.getRank()*localSize+i);
    }

    v1.add(scaleOther, v2);

    ASSERT_GT(v2.getLocalSize(), 1);

    for(unsigned long i=v1.getStartRow();i<v1.getStartRow()+v1.getLocalSize();++i) {
        ASSERT_FLOAT_EQ(v1(i), i-scaleOther*i);
    }
}

TEST(DistributedVectorTests, InPlaceVectorAddMul2) {

    const int localSize = 100;
    double scaleThis = 4322.2;
    double scaleOther = 432.2;
    MPI::MpiInfo mpiInfo = MPI::MpiInfo::Create();
    const int vectorLength = localSize*mpiInfo.getSize();


    // generate two random vectors
    LINALG::DistributedVector v1((unsigned long) vectorLength,
                                 static_cast<unsigned long>(localSize * mpiInfo.getRank()), localSize);
    LINALG::DistributedVector v2((unsigned long) vectorLength,
                                 static_cast<unsigned long>(localSize * mpiInfo.getRank()), localSize);

    for (int i=0;i<localSize;++i) {
        v1(static_cast<unsigned long>(mpiInfo.getRank() * localSize + i)) = mpiInfo.getRank()*localSize+i;
        v2(static_cast<unsigned long>(mpiInfo.getRank() * localSize + i)) = -(mpiInfo.getRank()*localSize+i);
    }

    v1.add(scaleOther, v2, scaleThis);

    ASSERT_GT(v2.getLocalSize(), 1);

    for(unsigned long i=v1.getStartRow();i<v1.getStartRow()+v1.getLocalSize();++i) {
        ASSERT_FLOAT_EQ(v1(i), scaleThis*i-scaleOther*i);
    }
}

TEST(DistributedVectorTests, InPlaceVectorSub) {

    const int localSize = 100;
    MPI::MpiInfo mpiInfo = MPI::MpiInfo::Create();
    const int vectorLength = localSize*mpiInfo.getSize();


    // generate two random vectors
    LINALG::DistributedVector v1((unsigned long) vectorLength,
                                 static_cast<unsigned long>(localSize * mpiInfo.getRank()), localSize);
    LINALG::DistributedVector v2((unsigned long) vectorLength,
                                 static_cast<unsigned long>(localSize * mpiInfo.getRank()), localSize);

    for (int i=0;i<localSize;++i) {
        v1(static_cast<unsigned long>(mpiInfo.getRank() * localSize + i)) = mpiInfo.getRank()*localSize+i;
        v2(static_cast<unsigned long>(mpiInfo.getRank() * localSize + i)) = -(mpiInfo.getRank()*localSize+i);
    }

    v1.sub(v2);

    ASSERT_GT(v2.getLocalSize(), 1);

    for(unsigned long i=v1.getStartRow();i<v1.getStartRow()+v1.getLocalSize();++i) {
        ASSERT_FLOAT_EQ(v1(i), 2*i);
    }
}

TEST(DistributedVectorTests, InPlaceVectorScale) {

    const int localSize = 100;
    double scale = 23424.2;
    MPI::MpiInfo mpiInfo = MPI::MpiInfo::Create();
    const int vectorLength = localSize*mpiInfo.getSize();


    // generate two random vectors
    LINALG::DistributedVector v1((unsigned long) vectorLength,
                                 static_cast<unsigned long>(localSize * mpiInfo.getRank()), localSize);

    for (int i=0;i<localSize;++i) {
        v1(static_cast<unsigned long>(mpiInfo.getRank() * localSize + i)) = mpiInfo.getRank()*localSize+i;
    }

    v1.scale(scale);

    ASSERT_GT(v1.getLocalSize(), 1);

    for(unsigned long i=v1.getStartRow();i<v1.getStartRow()+v1.getLocalSize();++i) {
        ASSERT_FLOAT_EQ(v1(i), scale*i);
    }
}