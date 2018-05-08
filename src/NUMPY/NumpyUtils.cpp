//
// Created by Amadeus Gebauer on 07.05.18.
//

#include <iostream>
#include "NumpyUtils.h"
#include "NumpyIOException.h"

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


    const short int headerLength = reinterpret_cast<short int &>(buffer[8]);



    std::string header ((unsigned long) headerLength, '\0');
    if(!fileStream.read(&header[0], headerLength)) {
        throw NUMPY::NumpyIOException();
    }

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

        unsigned long currentPosition = 0;
        unsigned long lastPosition = 0;
        while(true) {
            currentPosition = shapeStr.find(',', lastPosition);
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
