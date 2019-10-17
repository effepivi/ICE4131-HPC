cmake_minimum_required (VERSION 2.6)
project (Chapter4)

find_package(Threads REQUIRED)

add_executable(chapter4-1 chapter4-1.cxx)
add_executable(chapter4-2 chapter4-2.cxx)
add_executable(chapter4-3-1 chapter4-3-1.cxx)
add_executable(chapter4-3-2 chapter4-3-2.cxx)
add_executable(chapter4-3-3 chapter4-3-3.cxx)
add_executable(chapter4-3-4 chapter4-3-4.cxx)
add_executable(chapter4-3-5 chapter4-3-5.cxx)

TARGET_LINK_LIBRARIES(chapter4-3-3 Threads::Threads)
TARGET_LINK_LIBRARIES(chapter4-3-4 Threads::Threads)
TARGET_LINK_LIBRARIES(chapter4-3-5 Threads::Threads)

