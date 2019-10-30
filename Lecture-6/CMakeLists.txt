cmake_minimum_required (VERSION 3.1)
project (ICE4131-Lecture6)

set (CMAKE_CXX_STANDARD 11)


find_package(OpenMP REQUIRED)
find_package(MPI REQUIRED)

add_executable(Chap6-HelloWorld-MPI-1 Chap6-HelloWorld-MPI-1.cxx)
add_executable(Chap6-HelloWorld-MPI-2 Chap6-HelloWorld-MPI-2.cxx)
add_executable(Chap6-HelloWorld-MPI-3 Chap6-HelloWorld-MPI-3.cxx)
add_executable(Chap6-HelloWorld-MPI-4 Chap6-HelloWorld-MPI-4.cxx)
add_executable(Chap6-HelloWorld-MPI-5 Chap6-HelloWorld-MPI-5.cxx)
add_executable(Chap6-HelloWorld-MPI-6 Chap6-HelloWorld-MPI-6.cxx)
add_executable(Chap6-HelloWorld-MPI-7 Chap6-HelloWorld-MPI-7.cxx)

target_compile_options(Chap6-HelloWorld-MPI-3 PRIVATE ${OpenMP_CXX_FLAGS})
TARGET_LINK_LIBRARIES(Chap6-HelloWorld-MPI-3 OpenMP::OpenMP_CXX)

target_include_directories(Chap6-HelloWorld-MPI-4 PRIVATE ${MPI_INCLUDE_PATH})
target_compile_options(Chap6-HelloWorld-MPI-4     PRIVATE ${MPI_CXX_COMPILE_OPTIONS})
target_link_libraries(Chap6-HelloWorld-MPI-4  MPI::MPI_CXX ${MPI_CXX_LINK_FLAGS})

target_include_directories(Chap6-HelloWorld-MPI-5 PRIVATE ${MPI_INCLUDE_PATH})
target_compile_options(Chap6-HelloWorld-MPI-5     PRIVATE ${MPI_CXX_COMPILE_OPTIONS})
target_link_libraries(Chap6-HelloWorld-MPI-5  MPI::MPI_CXX ${MPI_CXX_LINK_FLAGS})

target_include_directories(Chap6-HelloWorld-MPI-6 PRIVATE ${MPI_INCLUDE_PATH})
target_compile_options(Chap6-HelloWorld-MPI-6     PRIVATE ${MPI_CXX_COMPILE_OPTIONS})
target_link_libraries(Chap6-HelloWorld-MPI-6  MPI::MPI_CXX ${MPI_CXX_LINK_FLAGS})

target_include_directories(Chap6-HelloWorld-MPI-7 PRIVATE ${MPI_INCLUDE_PATH})
target_compile_options(Chap6-HelloWorld-MPI-7     PRIVATE ${MPI_CXX_COMPILE_OPTIONS})
target_link_libraries(Chap6-HelloWorld-MPI-7  MPI::MPI_CXX ${MPI_CXX_LINK_FLAGS})
