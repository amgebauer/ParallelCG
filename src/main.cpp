#include <iostream>
#include "LINALG/SymmetricMatrix.h"
#include "CGSolver.h"
#include "NUMPY/NumpyUtils.h"
#include <ctime>
#include <limits>

struct InvokationInfo {
    std::string executionName;
    std::string matrixFileName;
    std::string vectorFileName;
    std::string solutionFileName;
    double epsilon;
    unsigned long max_iter;
    unsigned long problem_size;
    bool fixed_iterations;
};

void usage_serial(const std::string &program_name) {
    std::cout<<"There are two ways to call the programm"<<std::endl<<std::endl;
    std::cout<<"Problem from NumPy file:"<<std::endl<<std::endl;
    std::cout<<program_name<<" -A <A> -v <v> -x <x>"<<std::endl<<std::endl;
    std::cout<<"   <A>: Path to numpy matrix file"<<std::endl;
    std::cout<<"   <v>: Path to numpy vector file"<<std::endl;
    std::cout<<"   <x>: Path to numpy file where the result will be stored"<<std::endl<<std::endl;
    std::cout<<"Dummy Problem with specigic size:"<<std::endl<<std::endl;
    std::cout<<program_name<<" -N <N>"<<std::endl<<std::endl;
    std::cout<<"   <N>: Size of the problem"<<std::endl<<std::endl;
    std::cout<<"Additionally you can add the following optional parameters"<<std::endl<<std::endl;
    std::cout<<"   -e <eps>: Used criterion to stop the CG method"<<std::endl;
    std::cout<<"   -m <max_iter>: Maximum iterations"<<std::endl;
}

void usage_parallel(const std::string &program_name, MPI::MpiInfo info) {
    if(info.getRank() == 0) {
        std::cout<<"There are two ways to call the programm"<<std::endl<<std::endl;
        std::cout<<"Problem from NumPy file:"<<std::endl<<std::endl;
        std::cout<<"mpirun -n <X> "<<program_name<<" -A <A> -v <v> -x <x>"<<std::endl<<std::endl;
        std::cout<<"   <X>: Number of processes"<<std::endl;
        std::cout<<"   <v>: Path to numpy vector file"<<std::endl;
        std::cout<<"   <A>: Path to numpy matrix file"<<std::endl;
        std::cout<<"   <x>: Path to numpy file where the result will be stored"<<std::endl<<std::endl;
        std::cout<<"Dummy Problem with specigic size:"<<std::endl<<std::endl;
        std::cout<<program_name<<" -N <N>"<<std::endl<<std::endl;
        std::cout<<"   <X>: Number of processes"<<std::endl;
        std::cout<<"   <N>: Size of the problem"<<std::endl<<std::endl;
        std::cout<<"Additionally you can add the following optional parameters"<<std::endl<<std::endl;
        std::cout<<"   -e <eps>: Used criterion to stop the CG method"<<std::endl;
        std::cout<<"   -m <max_iter>: Maximum iterations"<<std::endl;
    }
}

void print_statistics(unsigned long problem_size,
                      int rank_size,
                      clock_t begin,
                      clock_t inited,
                      clock_t solved,
                      clock_t finish,
                      unsigned int iterations) {
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
    std::cout<<"Iterations: "<<iterations<<std::endl;
}

struct InvokationInfo parse_arguments(int argc, char* argv[]) {
    struct InvokationInfo info;
    info.epsilon = 1e-5;
    info.max_iter = std::numeric_limits<unsigned long>::max();
    info.executionName = argv[0];
    info.fixed_iterations = false;

    for (int i=2;i<argc;i+=2) {

        if(argv[i-1] == std::string("-N")) {
            info.problem_size = std::stoul(argv[i]);
        } else if(argv[i-1] == std::string("-e")) {
            info.epsilon = std::stod(argv[i]);
        } else if(argv[i-1] == std::string("-m")) {
            info.max_iter = std::stoul(argv[i]);
        } else if(argv[i-1] == std::string("-A")) {
            info.matrixFileName = argv[i];
        } else if(argv[i-1] == std::string("-v")) {
            info.vectorFileName = argv[i];
        } else if(argv[i-1] == std::string("-x")) {
            info.solutionFileName = argv[i];
        } else if(argv[i-1] == std::string("-fi")
                  && (argv[i] == std::string("1") || argv[i-1] == std::string("true"))) {
            info.fixed_iterations = true;
        }

    }

    std::cout<<info.fixed_iterations<<std::endl;

    return info;
}

int execute_serial_problem(struct InvokationInfo invokationInfo) {
    std::string solution_file;

    clock_t begin = clock();
    // create matrix and vectors
    LINALG::SymmetricMatrix matrix(0);
    LINALG::Vector vector(0), solution(0);

    bool useNumpy = false;

    if(!invokationInfo.matrixFileName.empty()
       && !invokationInfo.vectorFileName.empty()
       && !invokationInfo.solutionFileName.empty()) {
        // program is started with paths to numpy files
        useNumpy = true;

        // read matrix
        NUMPY::NumpyUtils::readFullSymmetricMatrix(invokationInfo.matrixFileName, matrix);

        // read vector
        NUMPY::NumpyUtils::readFullVector(invokationInfo.vectorFileName, vector);

        // resize solution vector
        solution.resize(vector.getSize());
    } else if(invokationInfo.problem_size > 0) {
        // the program is started with 2 argmuents, so the size of the dummy problem is specified
        unsigned long problem_size = invokationInfo.problem_size;
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
        usage_serial(invokationInfo.executionName);
        return 1;
    }

    clock_t init = clock();


    std::cout<<"Data initialized"<<std::endl;
    std::cout<<"Solve Ax=b"<<std::endl;

    // Solve
    unsigned int iterations = SOLVE::CGSolver::solveSerial(matrix,
             vector,
             solution,
             invokationInfo.epsilon,
             invokationInfo.max_iter,
             invokationInfo.fixed_iterations);

    std::cout<<"Solution found!"<< std::endl;

    clock_t solve = clock();

    // Store solution
    if(useNumpy) {
        NUMPY::NumpyUtils::writeFullVector(invokationInfo.solutionFileName, solution);
        std::cout<<"Solution written to outfile!"<< std::endl;
    }

    clock_t finish = clock();
    print_statistics(solution.getSize(), 1, begin, init, solve, finish, iterations);
    return 0;
}

int execute_parallel_problem(struct InvokationInfo invokationInfo, MPI::MpiInfo& info) {
    clock_t begin = clock();
    // create matrix and vectors
    LINALG::DistributedSymmetricMatrix matrix(0, 0, 0);
    LINALG::DistributedVector vector(0, 0, 0), solution(0, 0, 0);
    bool useNumpy = false;
    if(!invokationInfo.matrixFileName.empty()
       && !invokationInfo.vectorFileName.empty()
       && !invokationInfo.solutionFileName.empty()) {
        // program is started with paths to numpy files
        useNumpy = true;

        // read matrix
        NUMPY::NumpyUtils::readDistributedSymmetricMatrix(invokationInfo.matrixFileName, matrix, info);

        // read vector
        NUMPY::NumpyUtils::readDistributedVector(invokationInfo.vectorFileName, vector, info);

        // resize solution vector
        unsigned long startRow, localSize;
        info.getLocalProblemDims(vector.getSize(), startRow, localSize);
        solution.resize(vector.getSize(), startRow, localSize);
    } else if(invokationInfo.problem_size > 0) {
        // the program is started with 2 argmuents, so the size of the dummy problem is specified
        unsigned long startRow, localSize;
        info.getLocalProblemDims(vector.getSize(), startRow, localSize);
        matrix.resize(invokationInfo.problem_size, startRow, localSize);
        vector.resize(invokationInfo.problem_size, startRow, localSize);
        solution.resize(invokationInfo.problem_size, startRow, localSize);

        // initialize with dummy problem
        unsigned long endIndex = matrix.getLocalStartRow()+matrix.getLocalSize();
        for (unsigned long i=matrix.getLocalStartRow();i<endIndex;++i) {

            for (unsigned long j=i;j<matrix.getGlobalSize();++j) {
                matrix(i,j) = 10*i+j;
                if(i == j) matrix(i,j) += invokationInfo.problem_size;
            }
            vector(i) = i+1;
        }

    } else {
        usage_parallel(invokationInfo.executionName, info);
        return 1;
    }

    clock_t init = clock();

    if (info.getRank() == 0) {
        std::cout<<"Data initialized"<<std::endl;
        std::cout<<"Solve Ax=b"<<std::endl;
    }

    // Solve
    LINALG::Vector x_0(0);
    unsigned int iterations = SOLVE::CGSolver::solveParallel(matrix,
                                   vector,
                                   solution,
                                   x_0,
                                   1e-5,
                                   invokationInfo.max_iter,
                                   invokationInfo.fixed_iterations,
                                   info);

    if (info.getRank() == 0) {
        std::cout << "Solution found!" << std::endl;
    }

    clock_t solve = clock();
    // Store solution
    if(useNumpy) {
        NUMPY::NumpyUtils::writeDistributedVector(invokationInfo.solutionFileName, solution, info);

        if (info.getRank() == 0) {
            std::cout<<"Solution written to outfile!"<< std::endl;
        }
    }


    if (info.getRank() == 0) {
        clock_t finish = clock();
        print_statistics(solution.getSize(), info.getSize(), begin, init, solve, finish, iterations);
    }
    return 0;
}

int main(int argc, char* argv[]) {
    // init mpi
    MPI::MpiInfo info = MPI::MpiInfo::Init(&argc, &argv);
    struct InvokationInfo invokationInfo;
    try {
        invokationInfo = parse_arguments(argc, argv);
    } catch(...) {
        if(info.getSize() > 1) {
            usage_parallel(argv[0], info);
        } else {
            usage_serial(argv[0]);
        }
        return 1;
    }


    int return_code;
    if(info.getSize() == 1) {
        return_code = execute_serial_problem(invokationInfo);
    } else {
        return_code = execute_parallel_problem(invokationInfo, info);
    }

    info.finalize();
    return return_code;
}
