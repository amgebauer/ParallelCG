cmake_minimum_required(VERSION 3.8)
project(CGMethod)

find_package(MPI REQUIRED)

include_directories(${MPI_INCLUDE_PATH})

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


add_executable(CGMethod src/main.cpp src/CGSolver.cpp src/CGSolver.h src/LINALG/SymmetricMatrix.cpp src/LINALG/SymmetricMatrix.h src/LINALG/Vector.cpp src/LINALG/Vector.h src/LINALG/DimensionMismatch.h)

target_link_libraries(CGMethod ${MPI_LIBRARIES})


if(MPI_COMPILE_FLAGS)
    set_target_properties(CGMethod PROPERTIES
            COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()

if(MPI_LINK_FLAGS)
    set_target_properties(CGMethod PROPERTIES
            LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()