#import "MakePermutationMatrix.h"
#import "ReadFromFile.h"
#import "mpi.h"
#import "CostMatrixGenerator.h"
#include <iostream>
#define MCW MPI_COMM_WORLD
int main(int argc, char* argv[])
{
    int rank, size;
    int data;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MCW, &rank);
    MPI_Comm_size(MCW, &size);
    std::vector<std::vector<double>> matrix;

    if (rank == 0) {
        std::cout << "Reading in file" << std::endl;
        auto returnValue = ReadFromFile::ReadFile("../input");


        std::cout << "itterating through returned vector." << std::endl;
        for (int i = 0; i < returnValue.size(); i++) {
            std::cout << returnValue[i].GetX() << " " << returnValue[i].GetY() << " " << returnValue[i].GetName() << std::endl;
        }
        matrix = CostMatrixGenerator::GenerateCostMatrix(returnValue);


        std::cout << "Making Permutations" << std::endl;
        auto t = MakePermutationMatrix::MakePermutation(returnValue, rank, size);
        std::cout << "itterating through returned permutations." << std::endl;
        for (int i = 0; i < t.size(); i++) {
            for (int j = 0; j < t[i].size(); j++) {
                std::cout << t[i][j].GetName() << " ";
            }
            std::cout << std::endl;
        }
    }
    return 0;
}