//
// Created by Amadeus Gebauer on 16.04.18.
//

#include <cmath>
#include <iostream>
#include "CGSolver.h"

void SOLVE::CGSolver::solve(const LINALG::SymmetricMatrix &matrix,
                            const LINALG::Vector &vector,
                            LINALG::Vector &result,
                            const double epsilon) {

    double epsilonSquared = pow(epsilon, 2);

    LINALG::Vector r = vector-matrix*result;
    LINALG::Vector p = r;

    double r_squared = r*r;

    int iter = 0;
    while (r_squared > epsilonSquared) {
        std::cout<<"iter: "<<iter<<", res="<<r_squared<<" > "<<epsilonSquared<<std::endl;
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
}
