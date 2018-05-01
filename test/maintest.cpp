//
// Created by Amadeus Gebauer on 30.04.18.
//

#include <gtest/gtest.h>
#include "gtest-mpi-listener.h"
#include "../src/MPI/MpiInfo.h"


int main(int argc, char* argv[]) {

    testing::InitGoogleTest(&argc, argv);

    // Initialize MPI
    MPI::MpiInfo mpiInfo = MPI::MpiInfo::Init(&argc, &argv);

    // Add object that will finalize MPI on exit; Google Test owns this pointer
    ::testing::AddGlobalTestEnvironment(new MPIEnvironment(mpiInfo));

    // Get the event listener list.
    ::testing::TestEventListeners& listeners =
            ::testing::UnitTest::GetInstance()->listeners();

    // Remove default listener
    delete listeners.Release(listeners.default_result_printer());

    // Adds MPI listener; Google Test owns this pointer
    listeners.Append(new MPIMinimalistPrinter);

    return RUN_ALL_TESTS();
}