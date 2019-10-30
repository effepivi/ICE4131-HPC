project(ICP4131-Lab3)

cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

INCLUDE_DIRECTORIES(include)

add_executable(ImLib
    include/Image.h
    include/PthreadImage.h
    src/Image.cxx
    src/PthreadImage.cxx
)

add_executable(flip src/flip.cxx)
add_executable(log  src/log.cxx)

target_link_libraries(flip Threads::Threads)
target_link_libraries(log  Threads::Threads)
