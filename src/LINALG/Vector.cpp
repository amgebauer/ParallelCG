//
// Created by Amadeus Gebauer on 16.04.18.
//

#include <cmath>
#include "Vector.h"
#include "DimensionMismatch.h"
#include <stdexcept>

LINALG::Vector::Vector(const unsigned long size) : size(size), values(size) {}

double& LINALG::Vector::operator()(const unsigned long i) {
    if (i >= size) throw DimensionMismatch();
    return values[i];
}


const double &LINALG::Vector::get(const unsigned long i) const {
    if (i >= size) throw DimensionMismatch();
    return values[i];
}

unsigned long LINALG::Vector::getSize() const {
    return size;
}

LINALG::Vector LINALG::Vector::operator+(const LINALG::Vector &vector) const {
    if(getSize() != vector.getSize()) throw LINALG::DimensionMismatch();
    LINALG::Vector result(getSize());
    LINALG::Vector::add(*this, vector, result);
    return result;
}

LINALG::Vector LINALG::Vector::operator-(const LINALG::Vector &vector) const {
    if(getSize() != vector.getSize()) throw LINALG::DimensionMismatch();
    LINALG::Vector result(getSize());
    LINALG::Vector::sub(*this, vector, result);
    return result;
}

double LINALG::Vector::operator*(const LINALG::Vector &vector) const {
    if(getSize() != vector.getSize()) throw LINALG::DimensionMismatch();
    double sum = 0;
    for(unsigned long i=0;i<getSize();i++) {
        sum += get(i)*vector.get(i);
    }

    return sum;
}

void LINALG::Vector::add(double lambda, const LINALG::Vector &vector) {
    for (unsigned long i = 0;i<getSize();++i) {
        operator()(i) += lambda*vector.get(i);
    }
}

void LINALG::Vector::add(const LINALG::Vector &vector) {
    for (unsigned long i = 0;i<getSize();++i) {
        operator()(i) += vector.get(i);
    }
}

void LINALG::Vector::add(double lambdaOther, const LINALG::Vector &vector, double lambdaThis) {
    for (unsigned long i = 0;i<getSize();++i) {
        operator()(i) = get(i)*lambdaThis + lambdaOther*vector.get(i);
    }
}

void LINALG::Vector::sub(const LINALG::Vector &vector) {
    for (unsigned long i = 0;i<getSize();++i) {
        operator()(i) -= vector.get(i);
    }
}

void LINALG::Vector::add(const LINALG::Vector &v1, const LINALG::Vector &v2, LINALG::Vector &result) {
    unsigned long size = v1.getSize();
    for (unsigned long i = 0;i<size;++i) {
        result(i) = v1.get(i) + v2.get(i);
    }
}

void LINALG::Vector::sub(const LINALG::Vector &v1, const LINALG::Vector &v2, LINALG::Vector &result) {
    unsigned long size = v1.getSize();
    for (unsigned long i = 0;i<size;++i) {
        result(i) = v1.get(i) - v2.get(i);
    }
}

double LINALG::Vector::normSquared() const {
    double result = 0;
    for (unsigned long i = 0;i<size;++i) {
        result += std::pow(get(i), 2);
    }
    return result;
}

void LINALG::Vector::scale(const double lambda) {
    for (unsigned long i = 0;i<size;++i) {
        operator()(i) *= lambda;
    }
}
