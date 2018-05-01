//
// Created by Amadeus Gebauer on 01.05.18.
//

#include <gtest/gtest.h>
#include <cmath>
#include "../src/LINALG/SymmetricMatrix.h"
#include "../src/CGSolver.h"

TEST(CGSolverTest, SerialTest) {

    // generate positive definite matrix
    const int size = 100;

    LINALG::SymmetricMatrix matrix(size);
    LINALG::Vector vector(size), solution(size);

    for (unsigned long i=0;i<matrix.getSize();++i) {

        for (unsigned long j=i;j<matrix.getSize();++j) {
            matrix(i,j) = 10*i+j;
            if(i == j) matrix(i,j) += size;
        }
        vector(i) = i+1;
    }


    // solve equation system
    SOLVE::CGSolver::solveSerial(matrix, vector, solution, 1e-8);

    double error = sqrt((matrix*solution-vector).normSquared());

    ASSERT_LT(error, 1e-8);
}