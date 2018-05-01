//
// Created by Amadeus Gebauer on 30.04.18.
//

#ifndef CGMETHOD_DISTRIBUTEDVECTOR_H
#define CGMETHOD_DISTRIBUTEDVECTOR_H


#include <vector>
#include "Vector.h"
#include "../MPI/MpiInfo.h"

namespace LINALG {
    class DistributedVector {

    private:
        const unsigned long globalSize;
        const unsigned long startRow;
        const unsigned long localSize;
        std::vector<double> values;

    public:
        explicit DistributedVector(unsigned long globalSize,
                                   unsigned long startRow,
                                   unsigned long localSize);

        unsigned long getSize() const;
        unsigned long getStartRow() const;
        unsigned long getLocalSize() const;

        double& operator() (unsigned long i);
        DistributedVector operator+(const DistributedVector& vector) const;
        DistributedVector operator-(const DistributedVector& vector) const;

        double distributedProduct(const DistributedVector& vector) const;
        double distributedNormSquared() const;
        Vector getFull(MPI::MpiInfo& mpiInfo) const;

        const double& get(unsigned long i) const;

        void add(const LINALG::DistributedVector& vector);
        void add(double lambda, const LINALG::DistributedVector& vector);
        void add(double lambda, const LINALG::DistributedVector& vector, double lambdaThis);

        void sub(const LINALG::DistributedVector& vector);

        void scale(double lambda);


        static void add(const LINALG::DistributedVector& v1, const LINALG::DistributedVector& v2, LINALG::DistributedVector& result);
        static void sub(const LINALG::DistributedVector& v1, const LINALG::DistributedVector& v2, LINALG::DistributedVector& result);
    };
}



#endif //CGMETHOD_DISTRIBUTEDVECTOR_H
