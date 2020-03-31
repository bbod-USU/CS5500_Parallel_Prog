cmake_minimum_required(VERSION 3.15)
project(HW10)
set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_COMPILER mpicxx)
find_package(MPI REQUIRED)

add_definitions(-DOMPI_SKIP_MPICXX)
set(HEADER_FILES
        MakePermutationMatrix.h
        ReadFromFile.h
        City.h
        )
set(SOURCE_FILES
        MakePermutationMatrix.cpp
        ReadFromFile.cpp
        CostMatrixGenerator.cpp CostMatrixGenerator.h ParseMatrixForMPI.h SerializeCities.h partialPermutation.h)
set(CMAKE_CXX_STANDARD 17)

add_executable(HW10 ${HEADER_FILES} ${SOURCE_FILES} main.cpp)
set_property(TARGET HW10 PROPERTY CXX_STANDARD 11)

target_compile_options(HW10 PRIVATE -Wall -Wextra -pedantic -Wl,--stack,10000000 -O3)
target_link_libraries(HW10 PRIVATE MPI::MPI_C)
# Prepare a pre-build step to run clang-format over all the [ch]pp source files.
# Start by finding the location of the clang-format executable.
#
find_program(CLANG_FORMAT "clang-format")
if (CLANG_FORMAT)
    #
    # Need to take the simple source file locations used for the project and get their full
    # file system locations for use in putting together the clang-format command line
    #
    unset(SOURCE_FILES_PATHS)
    foreach(SOURCE_FILE ${HEADER_FILES} ${SOURCE_FILES} ${UNIT_TEST_FILES} main.cpp)
        get_source_file_property(WHERE ${SOURCE_FILE} LOCATION)
        set(SOURCE_FILES_PATHS ${SOURCE_FILES_PATHS} ${WHERE})
    endforeach()

    #
    # This creates the clang-format target/command
    #
    add_custom_target(
            ClangFormat
            COMMAND ${CLANG_FORMAT}
            -i
            -style=file
            ${SOURCE_FILES_PATHS}
    )
    #
    # This makes the clang-format target a dependency of the main GoogleTestIntro project
    #
    #    add_dependencies(Weights ClangFormat)
else()
    message("Unable to find clang-format")
endif()