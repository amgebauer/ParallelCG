//
// Created by Amadeus Gebauer on 30.04.18.
//

#include <gtest/gtest.h>
#include "../src/MPI/MpiInfo.h"

TEST(MpiTests, MoreThanOneProcessor) {

    MPI::MpiInfo info = MPI::MpiInfo::Create();

    ASSERT_GT(info.getSize(), 1);
}

TEST(MpiTests, DistributedDimensions) {
    unsigned long N, startRow, localSize;

    // different test examples
    MPI::MpiInfo info1 = MPI::MpiInfo::CreateFake(0, 2);
    N = 10;
    info1.getLocalProblemDims(N, startRow, localSize);
    ASSERT_EQ(startRow, 0);
    ASSERT_EQ(localSize, 5);

    MPI::MpiInfo info2 = MPI::MpiInfo::CreateFake(1, 2);
    N = 10;
    info2.getLocalProblemDims(N, startRow, localSize);
    ASSERT_EQ(startRow, 5);
    ASSERT_EQ(localSize, 5);


    MPI::MpiInfo info3 = MPI::MpiInfo::CreateFake(0, 2);
    N = 11;
    info3.getLocalProblemDims(N, startRow, localSize);
    ASSERT_EQ(startRow, 0);
    ASSERT_EQ(localSize, 6);


    MPI::MpiInfo info4 = MPI::MpiInfo::CreateFake(1, 2);
    N = 11;
    info4.getLocalProblemDims(N, startRow, localSize);
    ASSERT_EQ(startRow, 6);
    ASSERT_EQ(localSize, 5);

    MPI::MpiInfo info5 = MPI::MpiInfo::CreateFake(0, 3);
    N = 10;
    info5.getLocalProblemDims(N, startRow, localSize);
    ASSERT_EQ(startRow, 0);
    ASSERT_EQ(localSize, 4);


    MPI::MpiInfo info6 = MPI::MpiInfo::CreateFake(1, 3);
    N = 10;
    info6.getLocalProblemDims(N, startRow, localSize);
    ASSERT_EQ(startRow, 4);
    ASSERT_EQ(localSize, 3);


    MPI::MpiInfo info7 = MPI::MpiInfo::CreateFake(2, 3);
    N = 10;
    info7.getLocalProblemDims(N, startRow, localSize);
    ASSERT_EQ(startRow, 7);
    ASSERT_EQ(localSize, 3);
}