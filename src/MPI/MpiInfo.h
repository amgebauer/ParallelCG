//
// Created by Amadeus Gebauer on 30.04.18.
//

#ifndef CGMETHOD_MPIINFO_H
#define CGMETHOD_MPIINFO_H


namespace MPI {
    class MpiInfo {

    private:
        const int rank;
        const int size;

    public:
        explicit MpiInfo(int rank, int rankSize);

        int getRank() const;
        int getSize() const;

        bool finalize() const;

        static MPI::MpiInfo Init(int* argc, char*** argv);
        static MPI::MpiInfo Create();
        static MPI::MpiInfo CreateFake(int rank, int rankSize);
        void getLocalProblemDims(unsigned long N, unsigned long& startRow, unsigned long& localSize) const;
    };

}


#endif //CGMETHOD_MPIINFO_H
