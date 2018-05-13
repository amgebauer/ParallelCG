#include <iostream>
#include "LINALG/SymmetricMatrix.h"
#include "CGSolver.h"
#include "NUMPY/NumpyUtils.h"


void usage(const std::string &program_name, MPI::MpiInfo info) {
    if(info.getRank() == 0) {
        std::cout<<"There are two ways to call the programm"<<std::endl<<std::endl;
        std::cout<<"Problem from NumPy file:"<<std::endl<<std::endl;
        std::cout<<program_name<<" <A> <b> <x>"<<std::endl<<std::endl;
        std::cout<<"   <A>: path to numpy matrix file"<<std::endl;
        std::cout<<"   <b>: path to numpy vector file"<<std::endl;
        std::cout<<"   <x>: path to numpy file where the result will be stored"<<std::endl<<std::endl;
        std::cout<<"Dummy Problem with specigic size:"<<std::endl<<std::endl;
        std::cout<<program_name<<" -N <N>"<<std::endl<<std::endl;
        std::cout<<"   <N>: Size of the problem"<<std::endl;
    }
}

int main(int argc, char* argv[]) {
    std::cout<<"Welcome to CG"<<std::endl;

    // init mpi
    MPI::MpiInfo info = MPI::MpiInfo::Init(&argc, &argv);

    std::string solution_file;

    // create matrix and vectors
    LINALG::SymmetricMatrix matrix(0);
    LINALG::Vector vector(0), solution(0);
    if(argc == 4) {
        // program is started with 3 arguments, so the input and output files are specified in numpy format

        std::string matrix_file = argv[1];
        std::string rhs_file = argv[2];
        solution_file = argv[3];

        // read matrix
        NUMPY::NumpyUtils::readFullSymmetricMatrix(matrix_file, matrix);

        // read vector
        NUMPY::NumpyUtils::readFullVector(rhs_file, vector);

        // resize solution vector
        solution.resize(vector.getSize());
    } else if(argc == 3) {
        // the program is started with 2 argmuents, so the size of the dummy problem is specified
        std::cout<<" 3 is >"<<argv[1]<<"<"<<std::endl;
        if(argv[1] != static_cast<std::string>("-N")) {
            usage(argv[0], info);
            return 1;
        }

        unsigned long problem_size = std::stoul(argv[2]);
        matrix.resize(problem_size);
        vector.resize(problem_size);
        solution.resize(problem_size);

        // initialize with dummy problem
        for (unsigned long i=0;i<matrix.getSize();++i) {

            for (unsigned long j=i;j<matrix.getSize();++j) {
                matrix(i,j) = 10*i+j;
                if(i == j) matrix(i,j) += problem_size;
            }
            vector(i) = i+1;
        }

    } else {
        usage(argv[0], info);
        return 1;
    }


    std::cout<<"Data initialized"<<std::endl;
    std::cout<<"Solve Ax=b"<<std::endl;

    // Solve
    SOLVE::CGSolver::solveSerial(matrix, vector, solution, 1e-5);

    std::cout<<"Solution found!"<< std::endl;

    // Store soltuion
    if(argc == 4) {
        NUMPY::NumpyUtils::writeFullVector(solution_file, solution);
        std::cout<<"Solution written to outfile!"<< std::endl;
    }

    info.finalize();
    return 0;
}
