//
// Created by Amadeus Gebauer on 16.04.18.
//

#ifndef CGMETHOD_CGSOLVER_H
#define CGMETHOD_CGSOLVER_H

#include "LINALG/SymmetricMatrix.h"
#include "LINALG/Vector.h"

namespace SOLVE {

    class CGSolver {

    public:
        static void solve(const LINALG::SymmetricMatrix& matrix,
                          const LINALG::Vector& vector,
                          LINALG::Vector& result,
                          double epsilon);
    };

}




#endif //CGMETHOD_CGSOLVER_H
