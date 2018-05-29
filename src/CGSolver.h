//
// Created by Amadeus Gebauer on 16.04.18.
//

#ifndef CGMETHOD_CGSOLVER_H
#define CGMETHOD_CGSOLVER_H

#include "LINALG/SymmetricMatrix.h"
#include "LINALG/Vector.h"
#include "LINALG/DistributedVector.h"
#include "LINALG/DistributedSymmetricMatrix.h"
#include "MPI/MpiInfo.h"

namespace SOLVE {

    class CGSolver {

    public:
        static void solveSerial(const LINALG::SymmetricMatrix& matrix,
                          const LINALG::Vector& vector,
                          LINALG::Vector& result,
                          double epsilon,
                          unsigned long max_iterations);
        static void solveParallel(const LINALG::DistributedSymmetricMatrix& matrix,
                          const LINALG::DistributedVector& vector,
                          LINALG::DistributedVector& result,
                          const LINALG::Vector& x_0,
                          double epsilon,
                          unsigned long max_iterations,
                          MPI::MpiInfo& mpiInfo);
    };

}




#endif //CGMETHOD_CGSOLVER_H
