cmake_minimum_required(VERSION 3.15)
project(FinalProject)
set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_COMPILER mpicxx)
find_package(MPI REQUIRED)

add_definitions(-DOMPI_SKIP_MPICXX)
set(HEADER_FILES
        Serialize_Deserialize_Vector.h)
set(SOURCE_FILES
        )

add_executable(HW10 ${HEADER_FILES} ${SOURCE_FILES} main.cpp)
set_property(TARGET HW10 PROPERTY CXX_STANDARD 11)

target_compile_options(HW10 PRIVATE -Wall -Wextra -pedantic -Wl,--stack,10000000 -O3)
target_link_libraries(HW10 PRIVATE MPI::MPI_C)
