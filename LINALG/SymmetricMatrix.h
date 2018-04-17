//
// Created by Amadeus Gebauer on 16.04.18.
//

#ifndef CGMETHOD_MATRIX_H
#define CGMETHOD_MATRIX_H

#include <vector>
#include "Vector.h"

namespace LINALG {

    class SymmetricMatrix {
    private:
        const unsigned long size;
        std::vector<double> values;

        const unsigned long getGlobalIndex(unsigned long i, unsigned long j) const;
    public:
        explicit SymmetricMatrix(unsigned long size);

        double& operator()( unsigned long i, unsigned long j);
        const double& get( unsigned long i, unsigned long j) const;
        void set(unsigned long i, unsigned long j, double value);

        LINALG::Vector operator*(LINALG::Vector& vector) const;

        const unsigned long getSize() const;

        double vectorSquaredMul(LINALG::Vector vector) const;
    };
}



#endif //CGMETHOD_MATRIX_H
