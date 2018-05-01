cmake_minimum_required(VERSION 3.8)
project(CGMethod)

find_package(MPI REQUIRED)

include_directories(${MPI_INCLUDE_PATH})

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -ffast-math")

if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
        ${CMAKE_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()


add_executable(CGMethod src/main.cpp src/CGSolver.cpp src/CGSolver.h src/LINALG/SymmetricMatrix.cpp src/LINALG/SymmetricMatrix.h src/LINALG/Vector.cpp src/LINALG/Vector.h src/LINALG/DimensionMismatch.h src/LINALG/DistributedSymmetricMatrix.cpp src/LINALG/DistributedSymmetricMatrix.h src/LINALG/DistributedVector.cpp src/LINALG/DistributedVector.h src/MPI/MpiInfo.cpp src/MPI/MpiInfo.h src/Utils/Random.cpp src/Utils/Random.h)
add_executable(CGMethodTest test/maintest.cpp src/CGSolver.cpp src/CGSolver.h src/LINALG/SymmetricMatrix.cpp src/LINALG/SymmetricMatrix.h src/LINALG/Vector.cpp src/LINALG/Vector.h src/LINALG/DimensionMismatch.h src/LINALG/DistributedSymmetricMatrix.cpp src/LINALG/DistributedSymmetricMatrix.h src/LINALG/DistributedVector.cpp src/LINALG/DistributedVector.h src/MPI/MpiInfo.cpp src/MPI/MpiInfo.h test/gtest-mpi-listener.h test/VectorTests.cpp test/DistributedVectorTests.cpp test/MpiTests.cpp test/CGSolverTest.cpp test/SymmetricMatrixTests.cpp test/RandomTest.cpp src/Utils/Random.cpp src/Utils/Random.h)

target_link_libraries(CGMethod ${MPI_LIBRARIES})
target_link_libraries(CGMethodTest ${MPI_LIBRARIES})
target_link_libraries(CGMethodTest gtest_main)

add_test(NAME CGMethodTest COMMAND CGMethodTest)


if(MPI_COMPILE_FLAGS)
    set_target_properties(CGMethod PROPERTIES
            COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()

if(MPI_LINK_FLAGS)
    set_target_properties(CGMethod PROPERTIES
            LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()