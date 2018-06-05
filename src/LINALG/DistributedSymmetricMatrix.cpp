//
// Created by Amadeus Gebauer on 30.04.18.
//

#include "DistributedSymmetricMatrix.h"
#include <iostream>
#include <stdexcept>

LINALG::DistributedSymmetricMatrix::DistributedSymmetricMatrix(unsigned long size,
                                                               unsigned long startRow,
                                                               unsigned long localSize)
        : values(localSize*size-(localSize*localSize-localSize)/2), globalSize(size), startRow(startRow), localSize(localSize) {}

unsigned long LINALG::DistributedSymmetricMatrix::getLocalIndex(unsigned long i, unsigned long j) const {

    if (i < startRow || i >= startRow+localSize) {
        // this item is not available on this processor
        throw std::out_of_range("The requested value is not sitting on this processor");
    }
    if(i > j && j >= startRow && j < startRow+localSize) {
        unsigned long tmpVar = j;
        j = i;
        i = tmpVar;
    }

    // efficient version
    unsigned long I = (2*(i*globalSize+i*startRow)+i-2*startRow*globalSize-i*i-startRow*startRow-startRow)/2+j;

    if (j > startRow) {
        I -= i-startRow;
    }

    return I;
}

const double &LINALG::DistributedSymmetricMatrix::get(unsigned long i, unsigned long j) const {
    return values[getLocalIndex(i, j)];
}

double &LINALG::DistributedSymmetricMatrix::operator()(unsigned long i, unsigned long j) {
    return values[getLocalIndex(i, j)];
}

unsigned long LINALG::DistributedSymmetricMatrix::getLocalStartRow() const {
    return startRow;
}

unsigned long LINALG::DistributedSymmetricMatrix::getLocalSize() const {
    return localSize;
}

unsigned long LINALG::DistributedSymmetricMatrix::getGlobalSize() const {
    return globalSize;
}

LINALG::DistributedVector LINALG::DistributedSymmetricMatrix::operator*(const LINALG::Vector &vector) const {
    LINALG::DistributedVector result(globalSize, startRow, localSize);

    const unsigned long endRow = startRow+localSize;
    for (unsigned long i=startRow;i<endRow;++i) {

        for (unsigned long j=0;j<globalSize;++j) {

            result(i) += get(i, j) * vector.get(j);

        }

    }

    return result;
}

void LINALG::DistributedSymmetricMatrix::resize(unsigned long size, unsigned long startRow, unsigned long localSize) {
    values.resize(localSize*size-(localSize*localSize-localSize)/2, 0.0);
    this->startRow = startRow;
    this->localSize = localSize;
    this->globalSize = size;
}

