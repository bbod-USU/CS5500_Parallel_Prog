#import "CostMatrixGenerator.h"
//#import "MakePermutationMatrix.h"
#import "ReadFromFile.h"
#import "mpi.h"

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
    if (rank == 0) {
        std::cout << "Reading in file" << std::endl;
        cities = ReadFromFile::ReadFile("../input");
        citiesSize = cities.size();
        std::cout << "after set "<< rank << " has value " << citiesSize << "as size of city" << std::endl;
        //Make matrix
        matrix = CostMatrixGenerator::GenerateCostMatrix(cities);
    }
    MPI_Bcast(&citiesSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if(!rank){
        // reserve memory for vectors
        matrix.resize(citiesSize);
        for(int i = 0; i < citiesSize; i++) {
            matrix[i].resize(citiesSize);
        }
    }
    MPI_Bcast(&(matrix[0][0]), citiesSize*citiesSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    std::cout << "Process "<< rank << " has value " << citiesSize << "as size of city" << std::endl;

    if(rank){
        matrix[0][1]= 2.2;
        std::cout << "Process "<< rank << " has value " << citiesSize << "as size of city" << std::endl;
        for(int i = 0; i < citiesSize; i++){
            for(int j = 0; j < citiesSize; j++) {
                std::cout<<"Process " << rank << " has value " << matrix[i][j] << std::endl;
            }
        }
    }
    MPI_Finalize();
//    //Make permutations
//    auto perms = MakePermutationMatrix::GetLowestCost(cities, rank, size, matrix);
return 0;
}