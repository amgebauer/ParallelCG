//
// Created by Amadeus Gebauer on 07.05.18.
//

#include <iostream>
#include "NumpyUtils.h"
#include "NumpyIOException.h"
#include <string.h>
#include <stdexcept>
#include <sstream>

void NUMPY::NumpyUtils::readFullVector(std::string fileName, LINALG::Vector &vector) {
    std::vector<unsigned long> shape(0);

    std::ifstream numpyFile (fileName, std::ios::in | std::ios::binary);
    NUMPY::NumpyUtils::readHeader(numpyFile, shape);
    if(shape.size() != 1) {
        // this file contains not an vector
        throw NUMPY::NumpyIOException();
    }

    // resize vector
    vector.resize(0);
    char c[8];
    while(numpyFile.read(c, 8)) {
        vector.push_back(reinterpret_cast<double&>(c));
    }

    numpyFile.close();

}

void NUMPY::NumpyUtils::readFullSymmetricMatrix(std::string fileName, LINALG::SymmetricMatrix &matrix) {
    std::vector<unsigned long> shape(0);

    std::ifstream numpyFile (fileName, std::ios::in | std::ios::binary);
    NUMPY::NumpyUtils::readHeader(numpyFile, shape);
    if(shape.size() != 2) {
        // this file contains not an vector
        throw NUMPY::NumpyIOException();
    }

    // resize vector
    matrix.resize(shape[0]);
    char c[8];
    unsigned long size = shape[0];
    unsigned long i = 0;
    while(numpyFile.read(c, 8)) {
        matrix(i / size, i % size) = reinterpret_cast<double&>(c);
        ++i;
    }

    numpyFile.close();

}



void NUMPY::NumpyUtils::readHeader(std::istream &fileStream, std::vector<unsigned long>& shape) {
    char buffer[10];

    // read numpy magic string and file version
    if(!fileStream.read(buffer, 10)) {
        throw NUMPY::NumpyIOException();
    }


    unsigned char majorVersion = reinterpret_cast<unsigned char &>(buffer[6]);
    buffer[7] = '\0';

    // check magic string
    if(strcmp(buffer+1, "NUMPY") == 0) {
        throw NUMPY::NumpyIOException();
    }

    if(majorVersion != 1) {
        throw NUMPY::NumpyIOException();
    }


    const short int headerLength = reinterpret_cast<short int &>(buffer[8]);



    std::string header ((unsigned long) headerLength, '\0');
    if(!fileStream.read(&header[0], headerLength)) {
        throw NUMPY::NumpyIOException();
    }

    parseHeaderString(header, shape);
}

void NUMPY::NumpyUtils::readJsonOption(const std::string &json, const std::string &option, std::string &name) {

    std::string searchTarget = "'"+option+"':";

    unsigned long position = json.find(searchTarget)+searchTarget.length();


    // read from found position until end of option
    bool valueStarted = false;
    bool roundBracket = false;
    while (true) {
        char chr = json[position];
        if(valueStarted) {
            if(chr == '\'' || chr == '\0' || chr == '}' || chr == ')' || (chr == ',' && !roundBracket)) {
                break;
            } else {
                name.push_back(chr);
            }
        } else {

            if(chr == '\0') {
                break;
            } else if(chr == '\'') {
                valueStarted = true;
            } else if(chr == '(') {
                valueStarted = true;
                roundBracket = true;
            } else if(chr != ' ') {
                valueStarted = true;
                name.push_back(chr);
            }

        }

        ++position;
    }
}

void NUMPY::NumpyUtils::writeFullVector(std::string fileName, LINALG::Vector &vector) {

    std::vector<unsigned long> shape(1);
    shape[0] = vector.getSize();

    std::ofstream numpyFile(fileName, std::ios::out | std::ios::binary);

    writeHeader(numpyFile, shape);

    for (unsigned long i=0;i<vector.getSize();++i) {
        double num = vector(i);
        numpyFile.write((char*)&num, sizeof(double));
    }

    numpyFile.close();
}

void NUMPY::NumpyUtils::writeHeader(std::ostream &oStream, std::vector<unsigned long> &shape) {
    // write magic string
    oStream<<"\x93NUMPY";

    unsigned char majorVersion = 1;
    unsigned char minorVersion = 0;

    oStream<<majorVersion;
    oStream<<minorVersion;


    std::string shapeStr;
    for (unsigned long i : shape) {
        shapeStr += std::to_string(i) +",";
    }
    std::string header = "{'descr': '<f8', 'fortran_order': False, 'shape': ("+shapeStr+")}";


    while((10+header.size()-1) % 16 != 0) {
        header.append(" ");
    }
    header.append("\n");

    short int headerLength = static_cast<short>(header.size());
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"
    oStream.put(headerLength);
    oStream.put(headerLength >> 8);
#pragma clang diagnostic pop
    oStream<<header;
}

int NUMPY::NumpyUtils::getHeaderLength(std::vector<unsigned long> &shape) {
    std::ostringstream full_header;
    writeHeader(full_header, shape);
    std::string full_header_str = full_header.str();

    return static_cast<int>(full_header_str.length());
}

int NUMPY::NumpyUtils::writeHeaderParallel(MPI_File &file, std::vector<unsigned long> &shape, MPI_Status &status) {
    std::ostringstream full_header;
    writeHeader(full_header, shape);
    std::string full_header_str = full_header.str();

    int header_offset = static_cast<int>(full_header_str.length());
    if(MPI_File_write_at(file, 0, full_header_str.data(), header_offset, MPI_CHAR, &status) != MPI_SUCCESS) {
        throw NUMPY::NumpyIOException();
    }
    return header_offset;
}


int NUMPY::NumpyUtils::readHeaderParallel(MPI_File &file, std::vector<unsigned long> &shape, MPI_Status &status) {

    char buffer[10];

    // read numpy magic string, file version and header length
    if (MPI_File_read_at(file, 0, buffer, 10, MPI_CHAR, &status) != MPI_SUCCESS) {
        throw NUMPY::NumpyIOException();
    }


    unsigned char majorVersion = reinterpret_cast<unsigned char &>(buffer[6]);
    buffer[7] = '\0';

    // check magic string
    if(strcmp(buffer+1, "NUMPY") == 0) {
        throw NUMPY::NumpyIOException();
    }

    if(majorVersion != 1) {
        throw NUMPY::NumpyIOException();
    }

    const short int headerLength = reinterpret_cast<short int &>(buffer[8]);

    // read header string
    char header_buf[headerLength];
    if (MPI_File_read_at(file, 10, header_buf, headerLength, MPI_CHAR, &status) != MPI_SUCCESS) {
        throw NUMPY::NumpyIOException();
    }


    std::string header(header_buf);
    parseHeaderString(header, shape);
    return headerLength+10;
}

void NUMPY::NumpyUtils::readDistributedVector(std::string fileName, LINALG::DistributedVector &vector,
                                              MPI::MpiInfo &mpiInfo) {
    std::vector<unsigned long> shape;


    // open file
    MPI_File file;
    MPI_Status status;

    MPI_File_open(MPI_COMM_WORLD, fileName.c_str(),
                  MPI_MODE_RDONLY,
                  MPI_INFO_NULL, &file);


    int header_offset = readHeaderParallel(file, shape, status);


    if(shape.size() != 1 || shape[0] <= 0) {
        throw NUMPY::NumpyIOException();
    }


    unsigned long startRow, localSize;

    mpiInfo.getLocalProblemDims(shape[0], startRow, localSize);
    vector.resize(shape[0], startRow, localSize);


    // open the corresponding view of the output file
    int my_offset = static_cast<int>(header_offset + vector.getStartRow() * sizeof(double));
    if(MPI_File_set_view(file, my_offset, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL) != MPI_SUCCESS) {
        throw NUMPY::NumpyIOException();
    }


    // read my values of the vector
    char buffer[sizeof(double)*localSize];
    if(MPI_File_read(file, buffer, sizeof(double)*localSize, MPI_CHAR, &status) != MPI_SUCCESS) {
        throw NUMPY::NumpyIOException();
    }

    for (unsigned long i=startRow;i<startRow+localSize;++i) {
        char chararr[sizeof(double)];
        memcpy(chararr, buffer+sizeof(double)*(i-startRow), sizeof(double));
        vector(i) = reinterpret_cast<double&>(chararr);
    }

}

void NUMPY::NumpyUtils::readDistributedSymmetricMatrix(std::string fileName, LINALG::DistributedSymmetricMatrix &matrix,
                                                       MPI::MpiInfo &mpiInfo) {
    std::vector<unsigned long> shape;
    // open file
    MPI_File file;
    MPI_Status status;

    MPI_File_open(MPI_COMM_WORLD, fileName.c_str(),
                  MPI_MODE_RDONLY,
                  MPI_INFO_NULL, &file);


    int header_offset = readHeaderParallel(file, shape, status);


    if(shape.size() != 2 || shape[0] <= 0 || shape[1] != shape[0]) {
        throw NUMPY::NumpyIOException();
    }


    unsigned long startRow, localSize;

    mpiInfo.getLocalProblemDims(shape[0], startRow, localSize);
    matrix.resize(shape[0], startRow, localSize);


    // open the corresponding view of the output file
    int my_offset = static_cast<int>(header_offset + startRow*shape[0]* sizeof(double));
    if(MPI_File_set_view(file, my_offset, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL) != MPI_SUCCESS) {
        throw NUMPY::NumpyIOException();
    }


    // read my values of the vector
    char buffer[sizeof(double)*localSize*shape[0]];
    if(MPI_File_read(file, buffer, sizeof(double)*localSize*shape[0], MPI_CHAR, &status) != MPI_SUCCESS) {
        throw NUMPY::NumpyIOException();
    }

    for (unsigned long i=startRow;i<startRow+localSize;++i) {
        for (unsigned long j=0;j<shape[0];++j) {
            char chararr[sizeof(double)];
            memcpy(chararr, buffer+sizeof(double)*((i-startRow)*shape[0]+j), sizeof(double));
            matrix(i,j) = reinterpret_cast<double&>(chararr);
        }

    }
}

void
NUMPY::NumpyUtils::writeDistributedVector(std::string fileName, LINALG::DistributedVector &vector, MPI::MpiInfo info) {


    std::vector<unsigned long> shape(1);
    shape[0] = vector.getSize();

    int header_offset = getHeaderLength(shape);

    MPI_File file;
    MPI_Status status;

    // opening a file in write and create mode
    if(MPI_File_open(MPI_COMM_WORLD, fileName.c_str(),
                  MPI_MODE_WRONLY | MPI_MODE_CREATE,
                  MPI_INFO_NULL, &file) != MPI_SUCCESS) {
        throw NUMPY::NumpyIOException();
    }
    // defining the size of the file
    if(MPI_File_set_size(file, header_offset+sizeof(double)*vector.getSize()) != MPI_SUCCESS) {
        throw NUMPY::NumpyIOException();
    }

    if (info.getRank() == 0) {
        // write header
        writeHeaderParallel(file, shape, status);
    }

    // open the corresponding view of the output file
    int my_offset = static_cast<int>(header_offset + vector.getStartRow() * sizeof(double));
    if(MPI_File_set_view(file, my_offset, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL) != MPI_SUCCESS) {
        throw NUMPY::NumpyIOException();
    }

    // write file
    int my_size = static_cast<int>(vector.getLocalSize() * sizeof(double));
    char data[vector.getLocalSize() * sizeof(double)];

    unsigned long startRow = vector.getStartRow();
    unsigned long lastRow = startRow+vector.getLocalSize();
    for (unsigned long i=startRow;i<lastRow;++i) {
        double num = vector(i);
        memcpy(data+(i-startRow)* sizeof(double), &num, sizeof(double));
    }

    if(MPI_File_write(file, data, my_size, MPI_CHAR, &status) != MPI_SUCCESS) {
        throw NUMPY::NumpyIOException();
    }
    if(MPI_File_close(&file) != MPI_SUCCESS) {
        throw NUMPY::NumpyIOException();
    }
}

void NUMPY::NumpyUtils::parseHeaderString(std::string& header, std::vector<unsigned long> &shape) {


    std::string descr;
    readJsonOption(header, "descr", descr);

    // accept only double arrays
    if(descr != "<f8") {
        throw NUMPY::NumpyIOException();
    }

    std::string fortranOrder;
    readJsonOption(header, "fortran_order", fortranOrder);
    // accept only non-fortran arrays
    if(fortranOrder != "False") {
        throw NUMPY::NumpyIOException();
    }

    std::string shapeStr;
    readJsonOption(header, "shape", shapeStr);

    // parse shape
    try {

        unsigned long lastPosition = 0;
        while(true) {
            unsigned long currentPosition = shapeStr.find(',', lastPosition);
            if(currentPosition == std::string::npos) {
                currentPosition = shapeStr.length();
            }

            if(currentPosition <= lastPosition) {
                break;
            } else {
                unsigned long test = std::stoul(shapeStr.substr(lastPosition, currentPosition-lastPosition));
                shape.push_back(test);
            }

            lastPosition = currentPosition+1;
        }

    } catch (std::invalid_argument&) {
        throw NUMPY::NumpyIOException();
    } catch (std::out_of_range&) {
        throw NUMPY::NumpyIOException();
    }
}
