//
// Created by Amadeus Gebauer on 16.04.18.
//

#include <cmath>
#include <iostream>
#include "CGSolver.h"

unsigned int SOLVE::CGSolver::solveSerial(const LINALG::SymmetricMatrix &matrix,
                            const LINALG::Vector &vector,
                            LINALG::Vector &result,
                            const double epsilon,
                            const unsigned long max_iterations,
                            const bool fixed_iterations) {

    double epsilonSquared = epsilon*epsilon;

    if(fixed_iterations) {
        // make fixed iterations --> set epsilon negative so that it will never be reached
        epsilonSquared = -1;
    }

    LINALG::Vector r = vector-matrix*result;
    LINALG::Vector p = r;

    double r_squared = r*r;

    unsigned int iter = 0;
    while (r_squared > epsilonSquared && iter < max_iterations) {
        const LINALG::Vector tmpVector = matrix*p;

        double alpha = r_squared/(p*tmpVector);

        result.add(alpha, p);
        r.add(-alpha, tmpVector);

        double r_squared_new = r*r;
        double beta = r_squared_new / r_squared;
        r_squared = r_squared_new;

        p.add(1, r, beta);
        iter += 1;
    }
    return iter;
}

unsigned int SOLVE::CGSolver::solveParallel(const LINALG::DistributedSymmetricMatrix &matrix,
                                    const LINALG::DistributedVector &vector,
                                    LINALG::DistributedVector &result,
                                    const LINALG::Vector& x_0,
                                    const double epsilon,
                                    const unsigned long max_iterations,
                                    const bool fixed_iterations,
                                    MPI::MpiInfo& mpiInfo) {

    double epsilonSquared = epsilon*epsilon;

    if(fixed_iterations) {
        // make fixed iterations --> set epsilon negative so that it will never be reached
        epsilonSquared = -1;
    }

    LINALG::DistributedVector r = vector;
    if(x_0.getSize() > 0) {
        r.sub(matrix*x_0);
    }

    LINALG::DistributedVector p = r;

    double r_squared = r.distributedProduct(r);

    unsigned int iter = 0;
    while(r_squared > epsilonSquared && iter < max_iterations) {
        LINALG::Vector p_full = p.getFull(mpiInfo);
        const LINALG::DistributedVector tmpVector = matrix*p_full;

        double alpha = r_squared/(p.distributedProduct(tmpVector));

        result.add(alpha, p);
        r.add(-alpha, tmpVector);
        double r_squared_new = r.distributedProduct(r);
        double beta = r_squared_new / r_squared;
        r_squared = r_squared_new;

        p.add(1, r, beta);
        iter += 1;
    }

    return iter;
}
