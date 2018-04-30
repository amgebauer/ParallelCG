#include <iostream>
#include "LINALG/SymmetricMatrix.h"
#include "CGSolver.h"

std::ostream& operator<<(std::ostream& os, const LINALG::SymmetricMatrix& matrix)
{
    os<<"[\n   ";
    for(unsigned long i=0;i<matrix.getSize();++i) {

        if(i > 0) {
            os << "\n   ";
        }

        if(i < 3 || matrix.getSize() <= 5 || i+1 == matrix.getSize()) {
            for(unsigned long j=0;j<matrix.getSize();++j) {
                if(j > 0) {
                    os << ", ";
                }

                if (j < 3 || matrix.getSize() <= 5 || j+1 == matrix.getSize()) {
                    os << matrix.get(i,j);
                } else {
                    os << "...";
                    j = matrix.getSize()-2;
                }
            }
        }
        else {
            os << "       ...";
            i = matrix.getSize()-2;
        }
    }
    os << "\n]";
    return os;
}

std::ostream& operator<<(std::ostream& os, const LINALG::Vector& vector)
{
    os<<"[";
    for(unsigned long i=0;i<vector.getSize();++i) {
        if(i > 0) {
            os << ", ";
        }

        if(i < 3 || vector.getSize() <= 5 || i+1 == vector.getSize()) {
            os << vector.get(i);
        } else {
            os << "       ...";
            i = vector.getSize()-2;
        }

    }
    os << "]";
    return os;
}

int main(int argc, char* argv[]) {
    // init mpi
    MPI::MpiInfo info = MPI::MpiInfo::Init(&argc, &argv);

    LINALG::DistributedVector vector_test(2*static_cast<unsigned long>(info.getSize()),
                                      2*static_cast<unsigned long>(info.getRank()),
                                      2);

    vector_test(static_cast<unsigned long>(2*info.getRank())) = 2*info.getRank()+1;
    vector_test(static_cast<unsigned long>(2*info.getRank())+1) = 2*info.getRank()+2;

    std::cout<<vector_test(static_cast<unsigned long>(2*info.getRank()))<<std::endl;

    double result = vector_test.distributedProduct(vector_test, info);

    if(result != 4*info.getSize()) {
        std::cout<<"FEHLER!"<<std::endl;
        std::cout<<result<<" instead of "<<4*info.getSize()<<std::endl;
        return 1;
    }



    const int size = 5000;

    LINALG::SymmetricMatrix matrix(size);
    LINALG::Vector vector(size), solution(size);

    for (unsigned long i=0;i<matrix.getSize();++i) {

        for (unsigned long j=i;j<matrix.getSize();++j) {
            matrix(i,j) = 10*i+j;
            if(i == j) matrix(i,j) += size;
        }
        vector(i) = i+1;
    }

    std::cout<<"Solve Ax=b mit "<<std::endl<<"A = "<<matrix<<"\n\n"<<"b = "<<vector<<"\n\n";

    // Solve:
    SOLVE::CGSolver::solveSerial(matrix, vector, solution, 1e-5);

    std::cout<<"Solution found: x = "<<solution<< std::endl;

    info.finalize();
    return 0;
}
