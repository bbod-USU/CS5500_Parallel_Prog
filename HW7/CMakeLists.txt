cmake_minimum_required(VERSION 3.15)
project(HW7)

find_package(MPI REQUIRED)

add_definitions(-DOMPI_SKIP_MPICXX)
set(CMAKE_CXX_STANDARD 11)


add_executable(main main.cpp)
target_link_libraries(main PRIVATE MPI::MPI_C)

target_compile_options(main PRIVATE -Wall -Wextra -pedantic -Wl,--stack,10000000 -O3)
