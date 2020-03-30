//
// Created by Brady Bodily on 3/28/20.
//

#include "MakePermutationMatrix.h"
#include "mpi.h"
#include <algorithm>
#include <iostream>

std::vector<double> MakePermutationMatrix::MakePermutation(std::vector<City> source, int rank, int size, std::vector<std::vector<double>> costMatrix){
    std::vector<double> returnVector;
    std::vector<double> localCostVector;
    std::sort(source.begin(), source.end());
    std::vector<City> myPerm = source;
    std::vector<City> nextPerm = source;
    auto offset = (myPerm.size()+1)/size;

    //return vector should be the size of number of processors as they are going to only return their own smallest.
    if(rank)
        returnVector.resize(source.size(), 0);

    //the "nextPerm" is the permutation to stop at.
    std::rotate(nextPerm.begin(), nextPerm.begin()+offset*(rank+1), nextPerm.end());

    //rotate my perm.
    std::rotate(myPerm.begin(), myPerm.begin()+offset*rank,myPerm.end());

    do {
        double cost;
        //Add up the cost of the current permutation.
        std::cout << "myPerm.size() " << myPerm.size()  << std::endl;
        std::cout << "source.size() " << source.size()  << std::endl;
        std::cout << "offset " << offset << std::endl;

        for (int i = 1; i < myPerm.size(); i++) {
            cost += costMatrix[myPerm[i].GetName()][myPerm[i-1].GetName()];
            std::cout << myPerm[i].GetName() << " ";
        }
        std::cout << std::endl;
        //Push permutation cost to the localCostVector.
        localCostVector.push_back(cost);
    } while (std::next_permutation(myPerm.begin(), myPerm.end()) && myPerm != nextPerm);

    //find the lowest and add it to a lowest vector.
    auto lowest = std::vector<double>(1,localCostVector[0]);
    for(int i = 0; i < localCostVector.size(); i++) {
        if(localCostVector[i] < lowest[0])
            lowest[0] = localCostVector[i];
            std::cout << lowest[0] << " ";
    }
    std::cout<< std::endl;
    MPI_Gather(&localCostVector,1, MPI_DOUBLE, &returnVector,1, MPI_DOUBLE,0, MPI_COMM_WORLD);

    return returnVector;
}

std::vector<double> MakePermutationMatrix::GetLowestCost(std::vector<City> source, int rank, int size, std::vector<std::vector<double>> costMatrix){
    return MakePermutation(source, rank, size, costMatrix);
}