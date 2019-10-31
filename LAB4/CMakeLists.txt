project(ICP4131-Lab3)

cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(OpenMP REQUIRED)

INCLUDE_DIRECTORIES(../LAB3/include)
INCLUDE_DIRECTORIES(include)

add_library(ImLib
    ../LAB3/include/Image.h
    ../LAB3/include/PthreadImage.h
    ../LAB3/src/Image.cxx
    ../LAB3/src/PthreadImage.cxx

    include/OpenMPImage.h
    src/OpenMPImage.cxx
)

target_compile_options(ImLib PRIVATE ${OpenMP_CXX_FLAGS})
target_link_libraries(ImLib Threads::Threads OpenMP::OpenMP_CXX)

add_executable(flip src/flip.cxx)
add_executable(log  src/log.cxx)

target_link_libraries(flip ImLib)
target_link_libraries(log  ImLib)
