//
// Created by Amadeus Gebauer on 16.04.18.
//

#include <cmath>
#include <iostream>
#include "CGSolver.h"

void SOLVE::CGSolver::solveSerial(const LINALG::SymmetricMatrix &matrix,
                            const LINALG::Vector &vector,
                            LINALG::Vector &result,
                            const double epsilon) {

    double epsilonSquared = pow(epsilon, 2);

    LINALG::Vector r = vector-matrix*result;
    LINALG::Vector p = r;

    double r_squared = r*r;

    int iter = 0;
    while (r_squared > epsilonSquared) {
        const LINALG::Vector tmpVector = matrix*p; // compute here part of the matrix product

        double alpha = r_squared/(p*tmpVector); // exchange part of the scalar product with other nodes - EXCHANGE scalar

        result.add(alpha, p); // compute part of new x
        r.add(-alpha, tmpVector); // compute part of new r

        double r_squared_new = r*r; // compute part of scalar product and communicate part of it - EXCHANGE scalar
        double beta = r_squared_new / r_squared; // compute new beta
        r_squared = r_squared_new;  // override r_squared

        p.add(1, r, beta); // compute part of p and exchange with other nodes - EXCHANGE vector
        iter += 1;
    }

    // MPI IO for x
}

void SOLVE::CGSolver::solveParallel(const LINALG::DistributedSymmetricMatrix &matrix,
                                    const LINALG::DistributedVector &vector,
                                    LINALG::DistributedVector &result,
                                    const LINALG::Vector& x_0,
                                    const double epsilon,
                                    MPI::MpiInfo& mpiInfo) {

    double epsilonSquared = pow(epsilon, 2.0);

    LINALG::DistributedVector r = vector-matrix*x_0;
    LINALG::DistributedVector p = r;

    double r_squared = r.distributedProduct(r, mpiInfo);

    int iter = 0;
    while(r_squared > epsilonSquared) {
        LINALG::Vector p_full = p.getFull(mpiInfo);
        const LINALG::DistributedVector tmpVector = matrix*p_full;

        double alpha = r_squared/(p.distributedProduct(tmpVector, mpiInfo));

        result.add(alpha, p);
        r.add(-alpha, tmpVector);

        double r_squared_new = r.distributedProduct(r, mpiInfo);
        double beta = r_squared_new / r_squared;
        r_squared = r_squared_new;

        p.add(1, r, beta);
        iter += 1;
    }
}
