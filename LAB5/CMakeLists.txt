cmake_minimum_required (VERSION 3.1)
project (ICE4131-Lab5)

set (CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(OpenMP REQUIRED)
find_package(MPI REQUIRED)

INCLUDE_DIRECTORIES(${MPI_INCLUDE_PATH})

INCLUDE_DIRECTORIES(../LAB3/include)
INCLUDE_DIRECTORIES(../LAB4/include)
INCLUDE_DIRECTORIES(include)

add_library(ImLib
    ../LAB3/include/Image.h
    ../LAB3/include/PthreadImage.h
    ../LAB4/include/OpenMPImage.h
    ../LAB3/src/Image.cxx
    ../LAB3/src/PthreadImage.cxx
    ../LAB4/src/OpenMPImage.cxx

    include/MPIImage.h
    src/MPIImage.cxx
)

target_compile_options(ImLib PRIVATE ${OpenMP_CXX_FLAGS} ${MPI_CXX_COMPILE_OPTIONS})
target_link_libraries(ImLib Threads::Threads OpenMP::OpenMP_CXX MPI::MPI_CXX ${MPI_CXX_LINK_FLAGS})

add_executable(flip src/flip.cxx)
add_executable(log  src/log.cxx)

target_link_libraries(flip ImLib)
target_link_libraries(log  ImLib)
