cmake_minimum_required (VERSION 3.1)
project (ICE4131-Lecture7)

set (CMAKE_CXX_STANDARD 11)

find_package(MPI REQUIRED)

add_executable(Chap7-HelloWorld-MPI Chap7-HelloWorld-MPI.cxx)
target_include_directories(Chap7-HelloWorld-MPI PRIVATE ${MPI_INCLUDE_PATH})
target_compile_options(Chap7-HelloWorld-MPI     PRIVATE ${MPI_CXX_COMPILE_OPTIONS})
target_link_libraries(Chap7-HelloWorld-MPI MPI::MPI_CXX ${MPI_CXX_LINK_FLAGS})


add_executable(Chap7-Comm1-MPI Chap7-Comm1-MPI.cxx)
target_include_directories(Chap7-Comm1-MPI PRIVATE ${MPI_INCLUDE_PATH})
target_compile_options(Chap7-Comm1-MPI     PRIVATE ${MPI_CXX_COMPILE_OPTIONS})
target_link_libraries(Chap7-Comm1-MPI MPI::MPI_CXX ${MPI_CXX_LINK_FLAGS})


add_executable(Chap7-Comm2-MPI Chap7-Comm2-MPI.cxx)
target_include_directories(Chap7-Comm2-MPI PRIVATE ${MPI_INCLUDE_PATH})
target_compile_options(Chap7-Comm2-MPI     PRIVATE ${MPI_CXX_COMPILE_OPTIONS})
target_link_libraries(Chap7-Comm2-MPI MPI::MPI_CXX ${MPI_CXX_LINK_FLAGS})


add_executable(Chap7-Comm3-MPI Chap7-Comm3-MPI.cxx)
target_include_directories(Chap7-Comm3-MPI PRIVATE ${MPI_INCLUDE_PATH})
target_compile_options(Chap7-Comm3-MPI     PRIVATE ${MPI_CXX_COMPILE_OPTIONS})
target_link_libraries(Chap7-Comm3-MPI MPI::MPI_CXX ${MPI_CXX_LINK_FLAGS})


add_executable(Chap7-Comm4-MPI Chap7-Comm4-MPI.cxx)
target_include_directories(Chap7-Comm4-MPI PRIVATE ${MPI_INCLUDE_PATH})
target_compile_options(Chap7-Comm4-MPI     PRIVATE ${MPI_CXX_COMPILE_OPTIONS})
target_link_libraries(Chap7-Comm4-MPI MPI::MPI_CXX ${MPI_CXX_LINK_FLAGS})
