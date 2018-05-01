//
// Created by Amadeus Gebauer on 16.04.18.
//

#include "SymmetricMatrix.h"
#include <stdexcept>

LINALG::SymmetricMatrix::SymmetricMatrix(const unsigned long size) : size(size), values(size*(size+1)/2) {}

double& LINALG::SymmetricMatrix::operator()(unsigned long i, unsigned long j) {
    return values[getGlobalIndex(i, j)];
}

unsigned long LINALG::SymmetricMatrix::getGlobalIndex(unsigned long i, unsigned long j) const {
    // only right top matrix is in the values
    // swap variable if
    unsigned long tmpVar;
    if(i > j) {
        tmpVar = j;
        j = i;
        i = tmpVar;
    }

    if(i >= size) throw std::out_of_range("The index exceeds the bounds of the Matrix");



    tmpVar = (size*(size+1))/2-((size-i)*(size-i+1))/2+j-i;
    return tmpVar;
}


const double& LINALG::SymmetricMatrix::get(unsigned long i, unsigned long j) const {
    return values[getGlobalIndex(i, j)];
}

LINALG::Vector LINALG::SymmetricMatrix::operator*(LINALG::Vector& vector) const {
    LINALG::Vector result(getSize());

    const unsigned long vectorSize = vector.getSize();
    for (unsigned long i=0;i<vectorSize;++i) {

        for (unsigned long j=0;j<vectorSize;++j) {

            result(i) += get(i, j) * vector(j);

        }

    }

    return result;
}

double LINALG::SymmetricMatrix::vectorSquaredMul(const LINALG::Vector vector) const {
    double result = 0;

    const unsigned long vectorSize = vector.getSize();
    for (unsigned long i=0;i<vectorSize;++i) {

        for (unsigned long j=i;j<vectorSize;++j) {

            if(i == j) {
                result += get(i, j) * vector.get(j)*vector.get(i);
            } else {
                result += 2*get(i, j) * vector.get(j)*vector.get(i);
            }

        }

    }

    return result;
}

unsigned long LINALG::SymmetricMatrix::getSize() const {
    return size;
}