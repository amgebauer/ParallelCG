//
// Created by Amadeus Gebauer on 07.05.18.
//

#ifndef CGMETHOD_NUMPYUTILS_H
#define CGMETHOD_NUMPYUTILS_H

#include <string>
#include <fstream>
#include "../LINALG/Vector.h"
#include "../LINALG/SymmetricMatrix.h"

namespace NUMPY {

    class NumpyUtils {

    public:
        static void readFullVector(std::string fileName, LINALG::Vector& vector);
        static void readFullSymmetricMatrix(std::string fileName, LINALG::SymmetricMatrix& matrix);

        static void writeFullVector(std::string fileName, LINALG::Vector& vector);

        static void writeHeader(std::ostream& oStream, std::vector<unsigned long>& shape);
        static void readHeader(std::istream& fileStream, std::vector<unsigned long>& shape);

    private:
        static void readJsonOption(const std::string& json, const std::string& option, std::string& name);
    };

}


#endif //CGMETHOD_NUMPYUTILS_H
