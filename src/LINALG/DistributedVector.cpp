//
// Created by Amadeus Gebauer on 30.04.18.
//

#include "DistributedVector.h"
#include <cmath>
#include <mpi.h>
#include <iostream>

LINALG::DistributedVector::DistributedVector(unsigned long globalSize, unsigned long startRow,
                                             unsigned long localSize)
    : globalSize(globalSize), startRow(startRow), localSize(localSize), values(localSize){}

unsigned long LINALG::DistributedVector::getSize() const {
    return globalSize;
}

unsigned long LINALG::DistributedVector::getStartRow() const {
    return startRow;
}

unsigned long LINALG::DistributedVector::getLocalSize() const {
    return localSize;
}

double &LINALG::DistributedVector::operator()(unsigned long i) {
    if(i < startRow || i >= startRow+localSize) {
        throw std::out_of_range("The requested value is not sitting on this processor");
    }
    return values[i-startRow];
}

LINALG::DistributedVector LINALG::DistributedVector::operator+(const LINALG::DistributedVector &vector) const {
    LINALG::DistributedVector result(globalSize, startRow, localSize);
    LINALG::DistributedVector::add(*this, vector, result);
    return result;
}

LINALG::DistributedVector LINALG::DistributedVector::operator-(const LINALG::DistributedVector &vector) const {
    LINALG::DistributedVector result(globalSize, startRow, localSize);
    LINALG::DistributedVector::sub(*this, vector, result);
    return result;
}

void LINALG::DistributedVector::add(const LINALG::DistributedVector &v1,
                                    const LINALG::DistributedVector &v2,
                                    LINALG::DistributedVector &result) {
    const unsigned long startRow = v1.getStartRow();
    const unsigned long endRow = startRow+v1.getLocalSize();
    for (unsigned long i = startRow;i<endRow;++i) {
        result(i) = v1.get(i) + v2.get(i);
    }
}

void LINALG::DistributedVector::sub(const LINALG::DistributedVector &v1,
                                    const LINALG::DistributedVector &v2,
                                    LINALG::DistributedVector &result) {
    const unsigned long startRow = v1.getStartRow();
    const unsigned long endRow = startRow+v1.getLocalSize();
    for (unsigned long i = startRow;i<endRow;++i) {
        result(i) = v1.get(i) - v2.get(i);
    }
}

const double &LINALG::DistributedVector::get(unsigned long i) const {
    if(i < startRow || i >= startRow+localSize) {
        throw std::out_of_range("The requested value is not sitting on this processor");
    }
    return values[i-startRow];
}

void LINALG::DistributedVector::add(const LINALG::DistributedVector &vector) {
    const unsigned long endRow = startRow+localSize;
    for (unsigned long i = startRow;i<endRow;++i) {
        operator()(i) += vector.get(i);
    }
}

void LINALG::DistributedVector::add(const double lambda, const LINALG::DistributedVector &vector) {
    const unsigned long endRow = startRow+localSize;
    for (unsigned long i = startRow;i<endRow;++i) {
        operator()(i) += lambda*vector.get(i);
    }
}

void LINALG::DistributedVector::add(const double lambda, const LINALG::DistributedVector &vector, const double lambdaThis) {
    const unsigned long endRow = startRow+localSize;
    for (unsigned long i = startRow;i<endRow;++i) {
        operator()(i) = lambdaThis*get(i)+lambda*vector.get(i);
    }
}

void LINALG::DistributedVector::sub(const LINALG::DistributedVector &vector) {
    const unsigned long endRow = startRow+localSize;
    for (unsigned long i = startRow;i<endRow;++i) {
        operator()(i) -= vector.get(i);
    }
}

void LINALG::DistributedVector::scale(double lambda) {
    const unsigned long endRow = startRow+localSize;
    for (unsigned long i = startRow;i<endRow;++i) {
        operator()(i) *= lambda;
    }
}

double LINALG::DistributedVector::distributedProduct(const LINALG::DistributedVector &vector, MPI::MpiInfo& mpiInfo) const {
    double localProduct = 0.0;
    const unsigned long endRow = startRow+localSize;
    for (unsigned long i = startRow;i<endRow;++i) {
        localProduct += get(i)*vector.get(i);
    }

    double globalProduct = 0.0;

    MPI_Allreduce(&localProduct, &globalProduct, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    return globalProduct;
}

LINALG::Vector LINALG::DistributedVector::getFull(MPI::MpiInfo& mpiInfo) const {
    // communicate vector to all processes
    LINALG::Vector result(globalSize);

    if(globalSize == 0) return result;

    // round up integer division
    const int elementsPerProc = static_cast<const int>(1 + ((globalSize - 1) / mpiInfo.getSize()));

    // create receive buffer
    double* recv_buf = static_cast<double *>(malloc(sizeof(double) * elementsPerProc * mpiInfo.getSize()));

    // create send buffer
    double* send_buf = static_cast<double *>(malloc(sizeof(double) * elementsPerProc));

    // fill send buffer
    for(unsigned long i=0;i<localSize;++i) {
        send_buf[i] = get(startRow+i);
    }

    MPI_Allgather(send_buf, elementsPerProc, MPI_DOUBLE, recv_buf, elementsPerProc, MPI_DOUBLE, MPI_COMM_WORLD);

    // extract values from receiving array
    int gap = 0;
    int breakpoint = globalSize % mpiInfo.getSize();
    for(unsigned long i=0;i<globalSize;++i) {
        if(i > breakpoint*elementsPerProc && breakpoint > 0) {
            gap = static_cast<int>((i - breakpoint * elementsPerProc) / (elementsPerProc - 1));
        }
        result(i) = recv_buf[i+gap];
    }

    // free buffers
    free(recv_buf);
    free(send_buf);


    return result;
}

