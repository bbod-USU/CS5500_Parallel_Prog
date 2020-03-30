#include "CostMatrixGenerator.h"
//#import "MakePermutationMatrix.h"
#include "ReadFromFile.h"
#include "ParseMatrixForMPI.h"
#include "mpi.h"

#include <iostream>
#define MCW MPI_COMM_WORLD
int main(int argc, char* argv[])
{
    int rank, size, citiesSize;
    int data;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MCW, &rank);
    MPI_Comm_size(MCW, &size);
    std::vector<std::vector<double>> matrix;
    std::vector<City> cities;
    std::vector<double> flatMatrix;
    if (rank == 0) {
        std::cout << "Reading in file" << std::endl;
        cities = ReadFromFile::ReadFile("../input");
        citiesSize = cities.size();
        //Make matrix
        matrix = CostMatrixGenerator::GenerateCostMatrix(cities);
        for(int i =0; i < cities.size(); i++){
            for(int j = 0; j< cities.size(); j++){
                std::cout << matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
        flatMatrix = matrixTools::FlattenMatrix(matrix);
        for(int i = 0; i < flatMatrix.size(); i++){
            std::cout << flatMatrix[i] << " ";
        }
        std::cout<<std::endl;
    }
    MPI_Bcast(&citiesSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if(rank){
        // reserve memory for vectors
        matrix.resize(citiesSize);
        for(int i = 0; i < citiesSize; i++) {
            matrix[i].resize(citiesSize);
        }
    }
    std::cout << "before bcast" << std::endl;
    MPI_Bcast(&flatMatrix, flatMatrix.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
    if(rank){
        matrix = matrixTools::UnflattenMatrix(flatMatrix, citiesSize, citiesSize);
        std::cout << "after unflatten" << std::endl;
        std::cout << "Process "<< rank << " has value " << citiesSize << " as size of city" << std::endl;
        for(int i = 0; i < citiesSize; i++) {
            for (int j = 0; j < citiesSize; j++) {
                std::cout << "Process " << rank << " has value " << matrix[i][j] << " at postion"
                          << "(" << i << "," << j << ")" << std::endl;
            }
        }
    }
    MPI_Finalize();
//    //Make permutations
//    auto perms = MakePermutationMatrix::GetLowestCost(cities, rank, size, matrix);
return 0;
}