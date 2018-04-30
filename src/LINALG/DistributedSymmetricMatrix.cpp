//
// Created by Amadeus Gebauer on 30.04.18.
//

#include "DistributedSymmetricMatrix.h"

LINALG::DistributedSymmetricMatrix::DistributedSymmetricMatrix(unsigned long size,
                                                               unsigned long startRow,
                                                               unsigned long localSize)
        : values(size*localSize), globalSize(size), startRow(startRow), localSize(localSize) {}

unsigned long LINALG::DistributedSymmetricMatrix::getLocalIndex(unsigned long i, unsigned long j) const {

    if (i < startRow || i >= startRow+localSize) {
        // this item is not available on this processor
        throw std::out_of_range("The requested value is not sitting on this processor");
    }

    // ToDo: More advanced saving technique
    // right now we are not exploiting the symmetric structure of the matrix

    return (i-startRow)*globalSize+j;
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

