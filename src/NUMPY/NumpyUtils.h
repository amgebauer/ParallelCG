//
// Created by Amadeus Gebauer on 07.05.18.
//

#ifndef CGMETHOD_NUMPYUTILS_H
#define CGMETHOD_NUMPYUTILS_H

#include <string>
#include <fstream>
#include <mpi.h>
#include "../LINALG/Vector.h"
#include "../LINALG/SymmetricMatrix.h"
#include "../LINALG/DistributedVector.h"
#include "../LINALG/DistributedSymmetricMatrix.h"

namespace NUMPY {

    class NumpyUtils {

    public:
        static void readFullVector(std::string fileName, LINALG::Vector& vector);
        static void readFullSymmetricMatrix(std::string fileName, LINALG::SymmetricMatrix& matrix);

        static void writeFullVector(std::string fileName, LINALG::Vector& vector);

        static void readDistributedVector(std::string fileName,
                                          LINALG::DistributedVector& vector,
                                          MPI::MpiInfo& mpiInfo);
        static void readDistributedSymmetricMatrix(std::string fileName,
                                                   LINALG::DistributedSymmetricMatrix& matrix,
                                                   MPI::MpiInfo& mpiInfo);

        static void writeDistributedVector(std::string fileName, LINALG::DistributedVector& vector, MPI::MpiInfo info);

        static void writeHeader(std::ostream& oStream, std::vector<unsigned long>& shape);
        static void readHeader(std::istream& fileStream, std::vector<unsigned long>& shape);
        static void parseHeaderString(std::string& header, std::vector<unsigned long>& shape);

        static int getHeaderLength(std::vector<unsigned long>& shape);

        static int writeHeaderParallel(MPI_File& file, std::vector<unsigned long>& shape, MPI_Status& status);
        static int readHeaderParallel(MPI_File& file, std::vector<unsigned long>& shape, MPI_Status& status);

    private:
        static void readJsonOption(const std::string& json, const std::string& option, std::string& name);
    };

}


#endif //CGMETHOD_NUMPYUTILS_H
