cmake_minimum_required(VERSION 3.8)
project(CGMethod)

find_package(MPI REQUIRED)

include_directories(${MPI_INCLUDE_PATH})

set(CMAKE_CXX_STANDARD 11)


add_executable(CGMethod main.cpp CGSolver.cpp CGSolver.h LINALG/SymmetricMatrix.cpp LINALG/SymmetricMatrix.h LINALG/Vector.cpp LINALG/Vector.h LINALG/DimensionMismatch.h)

target_link_libraries(CGMethod ${MPI_LIBRARIES})


if(MPI_COMPILE_FLAGS)
    set_target_properties(CGMethod PROPERTIES
            COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()

if(MPI_LINK_FLAGS)
    set_target_properties(CGMethod PROPERTIES
            LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()