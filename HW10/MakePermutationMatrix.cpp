//
// Created by Brady Bodily on 3/28/20.
//

#include "MakePermutationMatrix.h"

#include <mpi.h>

std::vector<double> MakePermutationMatrix::MakePermutation(std::vector<City> source, int rank, int size, std::vector<std::vector<double>> costMatrix){
    std::vector<double> returnVector;
    std::vector<double> localCostVector;
    std::sort(source.begin(), source.end());
    std::vector<City> myPerm = source;
    std::vector<City> nextPerm = source;
    auto offset = myPerm.size()/size;
    if(!rank){
        returnVector.resize(source.size(), 0);
    }
    if(rank)
        std::rotate(nextPerm.begin(), nextPerm.begin()+offset*(rank+1), nextPerm.end());
    else
        nextPerm = source;

    if(rank)
        std::rotate(myPerm.begin(), myPerm.begin()+offset*rank,myPerm.end());

    do {
        double cost;
        for (int i = 1; i < myPerm.size(); i++) {
            cost += costMatrix[myPerm[i].GetName()][myPerm[i-1].GetName()];
        }
        localCostVector.push_back(cost);
    } while (std::next_permutation(myPerm.begin(), myPerm.end()), myPerm != nextPerm);
    auto lowest = std::vector<double>(1,localCostVector[0]);
    for(int i = 0; i < localCostVector.size(); i++) {
        if(localCostVector[i] < lowest[0])
            lowest[0] = localCostVector[i];
    }
    MPI_Gather(&localCostVector,1, MPI_DOUBLE, &returnVector,1, MPI_DOUBLE,0, MPI_COMM_WORLD);

    return returnVector;
}

std::vector<double> MakePermutationMatrix::GetLowestCost(std::vector<City> source, int rank, int size, std::vector<std::vector<double>> costMatrix){
    return MakePermutation(source, rank, size, costMatrix);
}