cmake_minimum_required (VERSION 3.1)
project (ICE4131-Lecture7)

set (CMAKE_CXX_STANDARD 11)

find_package(MPI REQUIRED)

add_executable(Chap7-HelloWorld-MPI Chap7-HelloWorld-MPI.cxx)

target_include_directories(Chap7-HelloWorld-MPI PRIVATE ${MPI_INCLUDE_PATH})
target_compile_options(Chap7-HelloWorld-MPI     PRIVATE ${MPI_CXX_COMPILE_OPTIONS})
target_link_libraries(Chap7-HelloWorld-MPI MPI::MPI_CXX ${MPI_CXX_LINK_FLAGS})
