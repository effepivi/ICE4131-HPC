project(ICP4131-Lab2)

cmake_minimum_required(VERSION 3.1)

INCLUDE_DIRECTORIES(include)

add_executable(flip
    include/Image.h
    include/PthreadImage.h
    src/Image.cxx
    src/PthreadImage.cxx
    src/flip.cxx)
