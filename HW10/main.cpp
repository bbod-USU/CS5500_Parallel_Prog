#include "CostMatrixGenerator.h"
#include "MakePermutationMatrix.h"
#include "ParseMatrixForMPI.h"
#include "ReadFromFile.h"
#include "SerializeCities.h"
#include "mpi.h"

#include <chrono>
#include <tclDecls.h>

#define MCW MPI_COMM_WORLD
int main(int argc, char* argv[])
{
    int rank, size, citiesSize;
    std::chrono::time_point<std::chrono::steady_clock> start, stop;
    long double lowestCost = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MCW, &rank);
    MPI_Comm_size(MCW, &size);
    std::vector<std::vector<double>> costMatrix;
    std::vector<City> cities;
    std::vector<double> flatMatrix;
    std::vector<int> serializedCity;
    if (rank == 0) {
        std::cout << "Reading in file" << std::endl;
        cities = ReadFromFile::ReadFile("../input");
        citiesSize = cities.size();
        //Make costMatrix
        costMatrix = CostMatrixGenerator::GenerateCostMatrix(cities);
        flatMatrix = matrixTools::FlattenMatrix(costMatrix);
        serializedCity = SerializeCities::Serialize(cities);
    }

    MPI_Bcast(&citiesSize, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank){
        // reserve memory for vectors
        flatMatrix.resize(citiesSize*citiesSize);
        cities.resize(citiesSize);
        serializedCity.resize(citiesSize*3);
        costMatrix.resize(citiesSize);
        for(int i = 0; i < citiesSize; i++) {
            costMatrix[i].resize(citiesSize);
        }
    }

    MPI_Bcast(&flatMatrix[0], flatMatrix.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&serializedCity[0], citiesSize*3, MPI_INT, 0, MPI_COMM_WORLD);

    std::vector<double> costs;
    if(rank){
        cities = SerializeCities::Deserialize(serializedCity);
        costMatrix = matrixTools::UnflattenMatrix(flatMatrix, citiesSize, citiesSize);
            //Make permutations

    }
    if(!rank)
        start = std::chrono::high_resolution_clock::now();
    lowestCost = MakePermutationMatrix::GetLowestCost(cities, rank, size, costMatrix);
    if(!rank){
        stop = std::chrono::high_resolution_clock::now();
        std::cout << "The lowest cost is " << lowestCost << std::endl;
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
        std::cout << "Found in " << duration.count() << " seconds." << std::endl;
    }
    MPI_Finalize();


return 0;
}