#include <iostream>
#include "LINALG/SymmetricMatrix.h"
#include "CGSolver.h"
#include "NUMPY/NumpyUtils.h"
#include <ctime>


void usage_serial(const std::string &program_name) {
    std::cout<<"There are two ways to call the programm"<<std::endl<<std::endl;
    std::cout<<"Problem from NumPy file:"<<std::endl<<std::endl;
    std::cout<<program_name<<" <A> <b> <x>"<<std::endl<<std::endl;
    std::cout<<"   <A>: Path to numpy matrix file"<<std::endl;
    std::cout<<"   <b>: Path to numpy vector file"<<std::endl;
    std::cout<<"   <x>: Path to numpy file where the result will be stored"<<std::endl<<std::endl;
    std::cout<<"Dummy Problem with specigic size:"<<std::endl<<std::endl;
    std::cout<<program_name<<" -N <N>"<<std::endl<<std::endl;
    std::cout<<"   <N>: Size of the problem"<<std::endl;
}

void usage_parallel(const std::string &program_name, MPI::MpiInfo info) {
    if(info.getRank() == 0) {
        std::cout<<"There are two ways to call the programm"<<std::endl<<std::endl;
        std::cout<<"Problem from NumPy file:"<<std::endl<<std::endl;
        std::cout<<"mpirun -n <X> "<<program_name<<" <A> <b> <x>"<<std::endl<<std::endl;
        std::cout<<"   <X>: Number of processes"<<std::endl;
        std::cout<<"   <b>: Path to numpy vector file"<<std::endl;
        std::cout<<"   <A>: Path to numpy matrix file"<<std::endl;
        std::cout<<"   <x>: Path to numpy file where the result will be stored"<<std::endl<<std::endl;
        std::cout<<"Dummy Problem with specigic size:"<<std::endl<<std::endl;
        std::cout<<program_name<<" -N <N>"<<std::endl<<std::endl;
        std::cout<<"   <X>: Number of processes"<<std::endl;
        std::cout<<"   <N>: Size of the problem"<<std::endl;
    }
}

void print_statistics(unsigned long problem_size, int rank_size, clock_t begin, clock_t inited, clock_t solved, clock_t finish) {
    unsigned long problem_size_per_proc = problem_size / rank_size;
    if (problem_size % rank_size != 0) {
        ++problem_size_per_proc;
    }
    std::cout<<"Statistics:"<<std::endl;
    std::cout<<"Problem size: "<<problem_size<<std::endl;
    std::cout<<"Number Processes: "<<rank_size<<std::endl;
    std::cout<<"Problem size/processor: "<<problem_size_per_proc<<std::endl;
    std::cout<<"Read Time: "<<std::scientific<<(double)(inited-begin)/CLOCKS_PER_SEC<<" s"<<std::endl;
    std::cout<<"Solve Time: "<<std::scientific<<(double)(solved-inited)/CLOCKS_PER_SEC<<" s"<<std::endl;
    std::cout<<"Write Time: "<<std::scientific<<(double)(finish-solved)/CLOCKS_PER_SEC<<" s"<<std::endl;
}

int execute_serial_problem(int argc, char* argv[0]) {
    std::string solution_file;

    clock_t begin = clock();
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
        usage_serial(argv[0]);
        return 1;
    }

    clock_t init = clock();


    std::cout<<"Data initialized"<<std::endl;
    std::cout<<"Solve Ax=b"<<std::endl;

    // Solve
    SOLVE::CGSolver::solveSerial(matrix, vector, solution, 1e-5);

    std::cout<<"Solution found!"<< std::endl;

    clock_t solve = clock();

    // Store solution
    if(argc == 4) {
        NUMPY::NumpyUtils::writeFullVector(solution_file, solution);
        std::cout<<"Solution written to outfile!"<< std::endl;
    }

    clock_t finish = clock();
    print_statistics(solution.getSize(), 1, begin, init, solve, finish);
    return 0;
}

int execute_parallel_problem(int argc, char* argv[0], MPI::MpiInfo& info) {
    std::string solution_file;
    clock_t begin = clock();
    // create matrix and vectors
    LINALG::DistributedSymmetricMatrix matrix(0, 0, 0);
    LINALG::DistributedVector vector(0, 0, 0), solution(0, 0, 0);
    if(argc == 4) {
        // program is started with 3 arguments, so the input and output files are specified in numpy format

        std::string matrix_file = argv[1];
        std::string rhs_file = argv[2];
        solution_file = argv[3];

        // read matrix
        NUMPY::NumpyUtils::readDistributedSymmetricMatrix(matrix_file, matrix, info);

        // read vector
        NUMPY::NumpyUtils::readDistributedVector(rhs_file, vector, info);

        // resize solution vector
        unsigned long startRow, localSize;
        info.getLocalProblemDims(vector.getSize(), startRow, localSize);
        solution.resize(vector.getSize(), startRow, localSize);
    } else if(argc == 3) {
        // the program is started with 2 argmuents, so the size of the dummy problem is specified
        unsigned long problem_size = std::stoul(argv[2]);
        unsigned long startRow, localSize;
        info.getLocalProblemDims(vector.getSize(), startRow, localSize);
        matrix.resize(problem_size, startRow, localSize);
        vector.resize(problem_size, startRow, localSize);
        solution.resize(problem_size, startRow, localSize);

        // initialize with dummy problem
        unsigned long endIndex = matrix.getLocalStartRow()+matrix.getLocalSize();
        for (unsigned long i=matrix.getLocalStartRow();i<endIndex;++i) {

            for (unsigned long j=i;j<matrix.getGlobalSize();++j) {
                matrix(i,j) = 10*i+j;
                if(i == j) matrix(i,j) += problem_size;
            }
            vector(i) = i+1;
        }

    } else {
        usage_parallel(argv[0], info);
        return 1;
    }

    clock_t init = clock();

    if (info.getRank() == 0) {
        std::cout<<"Data initialized"<<std::endl;
        std::cout<<"Solve Ax=b"<<std::endl;
    }

    // Solve
    LINALG::Vector x_0(0);
    SOLVE::CGSolver::solveParallel(matrix, vector, solution, x_0, 1e-5, info);

    if (info.getRank() == 0) {
        std::cout << "Solution found!" << std::endl;
    }

    clock_t solve = clock();
    // Store solution
    if(argc == 4) {
        NUMPY::NumpyUtils::writeDistributedVector(solution_file, solution, info);

        if (info.getRank() == 0) {
            std::cout<<"Solution written to outfile!"<< std::endl;
        }
    }


    if (info.getRank() == 0) {
        clock_t finish = clock();
        print_statistics(solution.getSize(), info.getSize(), begin, init, solve, finish);
    }
    return 0;
}

int main(int argc, char* argv[]) {
    // init mpi
    MPI::MpiInfo info = MPI::MpiInfo::Init(&argc, &argv);

    int return_code;
    if(info.getSize() == 1) {
        return_code = execute_serial_problem(argc, argv);
    } else {
        return_code = execute_parallel_problem(argc, argv, info);
    }

    info.finalize();
    return return_code;
}
