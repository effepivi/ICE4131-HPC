cmake_minimum_required (VERSION 3.1)
project (Chapter-5)

find_package(OpenMP REQUIRED)

add_executable(chapter5-1 chapter5-1.cxx)
add_executable(chapter5-2 chapter5-2.cxx)
add_executable(chapter5-3 chapter5-3.cxx)
add_executable(chapter5-4 chapter5-4.cxx)
add_executable(chapter5-5 chapter5-5.cxx)

target_compile_options(chapter5-2 PRIVATE ${OpenMP_CXX_FLAGS})
target_compile_options(chapter5-3 PRIVATE ${OpenMP_CXX_FLAGS})
target_compile_options(chapter5-4 PRIVATE ${OpenMP_CXX_FLAGS})
target_compile_options(chapter5-5 PRIVATE ${OpenMP_CXX_FLAGS})

TARGET_LINK_LIBRARIES(chapter5-2 OpenMP::OpenMP_CXX)
TARGET_LINK_LIBRARIES(chapter5-3 OpenMP::OpenMP_CXX)
TARGET_LINK_LIBRARIES(chapter5-4 OpenMP::OpenMP_CXX)
TARGET_LINK_LIBRARIES(chapter5-5 OpenMP::OpenMP_CXX)

