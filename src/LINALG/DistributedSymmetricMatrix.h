//
// Created by Amadeus Gebauer on 30.04.18.
//

#ifndef CGMETHOD_DISTRIBUTEDSYMMETRIXMATRIX_H
#define CGMETHOD_DISTRIBUTEDSYMMETRIXMATRIX_H

#include <vector>
#include "DistributedVector.h"
#include "Vector.h"

namespace LINALG {
    class DistributedSymmetricMatrix {

    private:
        std::vector<double> values;
        unsigned long globalSize;
        unsigned long startRow;
        unsigned long localSize;

        unsigned long getLocalIndex(unsigned long i, unsigned long j) const;

    public:
        explicit DistributedSymmetricMatrix(unsigned long size,
                                            unsigned long startRow,
                                            unsigned long localSize);

        void resize(unsigned long size, unsigned long startRow, unsigned long localSize);

        double &operator() (unsigned long i, unsigned long j);
        const double& get(unsigned long i, unsigned long j) const;

        LINALG::DistributedVector operator*(const LINALG::Vector& vector) const;

        unsigned long getGlobalSize() const;
        unsigned long getLocalSize() const;
        unsigned long getLocalStartRow() const;
    };
}


#endif //CGMETHOD_DISTRIBUTEDSYMMETRIXMATRIX_H
