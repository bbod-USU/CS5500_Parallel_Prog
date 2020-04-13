cmake_minimum_required(VERSION 3.15)
project(FinalProject)
set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_COMPILER mpicxx)
find_package(MPI REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

add_definitions(-DOMPI_SKIP_MPICXX)
set(HEADER_FILES
        )
set(SOURCE_FILES
        )

add_executable(HW10 ${HEADER_FILES} ${SOURCE_FILES} main.cpp)
set_property(TARGET HW10 PROPERTY CXX_STANDARD 11)

target_compile_options(HW10 PRIVATE -Wall -Wextra -pedantic -Wl,--stack,10000000 -O3)
target_link_libraries(HW10 PRIVATE MPI::MPI_C nlohmann_json::nlohmann_json)
