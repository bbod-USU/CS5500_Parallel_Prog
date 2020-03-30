#include "CostMatrixGenerator.h"
#include "MakePermutationMatrix.h"
#include "ParseMatrixForMPI.h"
#include "ReadFromFile.h"
#include "mpi.h"

#define MCW MPI_COMM_WORLD
int main(int argc, char* argv[])
{
    int rank, size, citiesSize;
    int data;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MCW, &rank);
    MPI_Comm_size(MCW, &size);
    std::vector<std::vector<double>> costMatrix;
    std::vector<City> cities;
    std::vector<double> flatMatrix;
    if (rank == 0) {
        std::cout << "Reading in file" << std::endl;
        cities = ReadFromFile::ReadFile("../input");
        citiesSize = cities.size();
        //Make costMatrix
        costMatrix = CostMatrixGenerator::GenerateCostMatrix(cities);
//        for(int i =0; i < cities.size(); i++){
//            for(int j = 0; j< cities.size(); j++){
//                std::cout << costMatrix[i][j] << " ";
//            }
//            std::cout << std::endl;
//        }
        flatMatrix = matrixTools::FlattenMatrix(costMatrix);
        std::cout << "Here: ";
        for(int i = 0; i < cities.size(); i++){
            std::cout << cities[i].GetName() << " ";
        }
        std::cout << std::endl;
    }
    MPI_Bcast(&citiesSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if(rank){
        // reserve memory for vectors
        flatMatrix.resize(citiesSize*citiesSize);
        cities.resize(citiesSize);
        costMatrix.resize(citiesSize);
        for(int i = 0; i < citiesSize; i++) {
            costMatrix[i].resize(citiesSize);
        }
    }
    MPI_Bcast(&flatMatrix[0], flatMatrix.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(cities.data(), cities.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);

    std::vector<double> costs;
    if(rank){
        costMatrix = matrixTools::UnflattenMatrix(flatMatrix, citiesSize, citiesSize);
            //Make permutations
            std::cout << "source size from main " << cities.size() << std::endl;
        costs = MakePermutationMatrix::GetLowestCost(cities, rank, size, costMatrix);

    }
//    if(!rank){
//        std::cout << costs.size() << std::endl;
//        std::cout << costs[0];
//    }

    MPI_Finalize();

return 0;
}