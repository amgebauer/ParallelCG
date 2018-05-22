//
// Created by Amadeus Gebauer on 30.04.18.
//

#include "MpiInfo.h"
#include <mpi.h>
#include <algorithm>

MPI::MpiInfo::MpiInfo(const int rank, const int size)
: rank(rank), size(size){}

int MPI::MpiInfo::getRank() const {
    return rank;
}

int MPI::MpiInfo::getSize() const {
    return size;
}

MPI::MpiInfo MPI::MpiInfo::Init(int* argc, char*** argv) {
    MPI_Init(argc, argv);
    return MPI::MpiInfo::Create();
}

MPI::MpiInfo MPI::MpiInfo::Create() {
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MpiInfo info(rank, size);

    return info;
}

bool MPI::MpiInfo::finalize() const {
    return MPI_Finalize() == MPI_SUCCESS;
}

MPI::MpiInfo MPI::MpiInfo::CreateFake(int rank, int rankSize) {
    return MPI::MpiInfo(rank, rankSize);
}

void MPI::MpiInfo::getLocalProblemDims(unsigned long N, unsigned long &startRow, unsigned long &localSize) const {
    int overSizedProcs = static_cast<int>(N % size);
    localSize = N / size;


    if(rank < overSizedProcs) {
        ++localSize;
    }

    startRow = getRank()*localSize+std::min(rank, overSizedProcs);
}
