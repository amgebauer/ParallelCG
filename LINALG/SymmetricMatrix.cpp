//
// Created by Amadeus Gebauer on 16.04.18.
//

#include "SymmetricMatrix.h"

LINALG::SymmetricMatrix::SymmetricMatrix(const unsigned long size) : size(size), values(size*(size+1)/2) {}

double& LINALG::SymmetricMatrix::operator()(unsigned long i, unsigned long j) {
    return values[getGlobalIndex(i, j)];
}

const unsigned long LINALG::SymmetricMatrix::getGlobalIndex(unsigned long i, unsigned long j) const {
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

    for (unsigned long i=0;i<vector.getSize();++i) {

        for (unsigned long j=0;j<vector.getSize();++j) {

            result(i) += get(i, j) * vector(j);

        }

    }

    return result;
}

double LINALG::SymmetricMatrix::vectorSquaredMul(const LINALG::Vector vector) const {
    double result = 0;

    for (unsigned long i=0;i<vector.getSize();++i) {

        for (unsigned long j=i;j<vector.getSize();++j) {

            if(i == j) {
                result += get(i, j) * vector.get(j)*vector.get(i);
            } else {
                result += 2*get(i, j) * vector.get(j)*vector.get(i);
            }

        }

    }

    return result;
}

const unsigned long LINALG::SymmetricMatrix::getSize() const {
    return size;
}