cmake_minimum_required (VERSION 3.1)
project (ICE4131-Lab7)

# Enable C++ 11
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find CUDA
find_package(CUDA REQUIRED)

# Our CUDA program
cuda_add_executable(example1 example1.cu)
