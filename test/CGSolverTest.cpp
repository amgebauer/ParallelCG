//
// Created by Amadeus Gebauer on 01.05.18.
//

#include <gtest/gtest.h>
#include <cmath>
#include "../src/LINALG/SymmetricMatrix.h"
#include "../src/CGSolver.h"
#include "../src/Utils/Random.h"

TEST(CGSolverTest, SerialTest) {

    // generate positive definite matrix
    const int size = 100;

    LINALG::SymmetricMatrix matrix(size);
    LINALG::Vector vector(size), solution(size);

    for (unsigned long i=0;i<matrix.getSize();++i) {

        for (unsigned long j=i;j<matrix.getSize();++j) {
            matrix(i,j) = UTIL::Random::getRandom(-10,10);
            if(i == j) matrix(i,j) += UTIL::Random::getRandom(size+11,size+20);
        }
        vector(i) = UTIL::Random::getRandom(-10,10);
    }


    // solve equation system
    SOLVE::CGSolver::solveSerial(matrix, vector, solution, 1e-8);

    double error = sqrt((matrix*solution-vector).normSquared());

    ASSERT_LT(error, 1e-8);
}

/*TEST(CGSolverTest, ParallelTest) {
    MPI::MpiInfo info = MPI::MpiInfo::Create();

    for (unsigned long size=100;size<110;++size) {

        unsigned long mySize = size / info.getSize();
        unsigned long breakpoint = size % info.getSize();
        unsigned long myStart = mySize*info.getRank()+breakpoint;
        if(breakpoint > 0 && info.getRank() < breakpoint) {
            ++mySize;
        }

        // build serial and parallel matrices and vectors
        LINALG::SymmetricMatrix serialMatrix(size);
        LINALG::DistributedSymmetricMatrix parallelMatrix(size, myStart, mySize);
        LINALG::Vector serialVector(size);
        LINALG::Vector serialSolution(size);
        LINALG::Vector x_0(size);
        LINALG::DistributedVector parallelVector(size, myStart, mySize);
        LINALG::DistributedVector parallelSolution(size, myStart, mySize);

        // fill matrices and vectors
        for (unsigned long i=0;i<size;++i) {

            for (unsigned long j=i;j<size;++j) {

                serialMatrix(i,j) = (i+1)*(j+1);
                if(i == j) serialMatrix(i,j) += size;

                if(i >= myStart && i < myStart+mySize) {
                    parallelMatrix(i,j) = (i+1)*(j+1);
                    if(i == j) parallelMatrix(i,j) += size;
                }
            }

            serialVector(i) = (i+1);
            if(i >= myStart && i < myStart+mySize) {
                parallelVector(i) = (i+1);
            }
        }

        // First solve serial reference system
        SOLVE::CGSolver::solveSerial(serialMatrix, serialVector, serialSolution, 1e-8);

        // Solve parallel system
        SOLVE::CGSolver::solveParallel(parallelMatrix, parallelVector, parallelSolution, x_0, 1e-8, info);

        // check both solutions
        double serialError = sqrt((serialMatrix*serialSolution-serialVector).normSquared());
        ASSERT_LT(serialError, 1e-8);

        LINALG::Vector fullParallelSolution = parallelSolution.getFull(info);
        double parallelError = sqrt((parallelMatrix*fullParallelSolution-parallelVector).distributedNormSquared(info));
        ASSERT_LT(parallelError, 1e-8);

        // compare solutions
        for (unsigned long i = 0; i < size; ++i) {
            ASSERT_NEAR(serialSolution(i), fullParallelSolution(i), 2e-8);
        }
    }
}*/