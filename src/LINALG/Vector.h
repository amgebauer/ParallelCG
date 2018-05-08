//
// Created by Amadeus Gebauer on 16.04.18.
//

#ifndef CGMETHOD_VECTOR_H
#define CGMETHOD_VECTOR_H

#include <vector>
#include <string>
#include <ostream>

namespace LINALG {

    class Vector {
    private:
        unsigned long size;
        std::vector<double> values;
    public:
        explicit Vector(unsigned long size);

        unsigned long getSize() const;

    public:
        double& operator()(unsigned long i);
        Vector operator+(const Vector& vector) const;
        Vector operator-(const Vector& vector) const;
        double operator*(const Vector& vector) const;

        const double& get(unsigned long i) const;

        void add(const LINALG::Vector& vector);
        void add(double lambda, const LINALG::Vector& vector);
        void add(double lambdaOther, const LINALG::Vector& vector, double lambdaThis);

        void sub(const LINALG::Vector& vector);

        void scale(double lambda);

        double normSquared() const;

        void resize(unsigned long newSize);
        void push_back(double value);

        //friend std::ostream&operator<<(std::ostream& os, const Vector& vector);


        static void add(const LINALG::Vector& v1, const LINALG::Vector& v2, LINALG::Vector& result);
        static void sub(const LINALG::Vector& v1, const LINALG::Vector& v2, LINALG::Vector& result);


    };

}

#endif //CGMETHOD_VECTOR_H
