project(ICP4131-Lab3)

cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(my_app Threads::Threads)

INCLUDE_DIRECTORIES(include)

add_executable(flip
    include/Image.h
    include/PthreadImage.h
    src/Image.cxx
    src/PthreadImage.cxx
    src/flip.cxx)

target_link_libraries(flip Threads::Threads)

