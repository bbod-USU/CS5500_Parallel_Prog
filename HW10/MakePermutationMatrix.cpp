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
    std::size_t counter = 0;
    int vectorOffset = (source.size()+1)/2;
    std::sort(source.begin(), source.end());
    std::vector<City> myPerm = source;
    std::vector<City> nextPerm = source;

    //return vector should be the size of number of processors as they are going to only return their own smallest.
    returnVector.resize(source.size(), 0);

    //the "nextPerm" is the permutation to stop at.
    std::rotate(nextPerm.begin(), nextPerm.begin()+(rank+1), nextPerm.end());

    //rotate my perm.
    std::rotate(myPerm.begin(), myPerm.begin()+rank,myPerm.end());
    //std::cout << "source.last(): " << source[8].GetName() << std::endl;
    std::cout << "source: ";
    for(int i = 0; i < source.size(); i++){
        std::cout << source[i].GetName() << " ";
    }
    std::cout << std::endl << std::endl;
    std::cout <<"Rank: " << rank << " next perm: ";
    for(int i = 0; i < nextPerm.size(); i++){
        std::cout << nextPerm[i].GetName() << " ";
    }
    std::cout << std::endl;
    do {
        double cost = 0;
        //Add up the cost of the current permutation.
        for (int i = 0; i < myPerm.size(); i++) {
            std::cout << myPerm[i].GetName() << " ";
            cost += costMatrix[myPerm[i].GetName()][myPerm[i-1].GetName()];
        }
        std::cout << std::endl << std::endl;

        //Push permutation cost to the localCostVector.
        localCostVector.push_back(cost);
        counter++;
    } while (std::next_permutation(myPerm.begin(), myPerm.end()));

//    //find the lowest and add it to a lowest vector.
//    auto lowest = std::vector<double>(1,localCostVector[0]);
//    for(int i = 0; i < localCostVector.size(); i++) {
//        if(localCostVector[i] < lowest[0])
//            lowest[0] = localCostVector[i];
//            std::cout << lowest[0] << " ";
//    }

    std::cout<< "Rank: "<< rank<< "cost count: "<< localCostVector.size() << std::endl;
    std::cout << "Rank: "<< rank <<" Last Perm: ";
    for(int i = 0; i < myPerm.size(); i++){
        std::cout << myPerm[i].GetName() << " ";
    }
    std::cout << std::endl;
    //MPI_Gather(&localCostVector,1, MPI_DOUBLE, &returnVector,1, MPI_DOUBLE,0, MPI_COMM_WORLD);

    return returnVector;
}

std::vector<double> MakePermutationMatrix::GetLowestCost(std::vector<City> source, int rank, int size, std::vector<std::vector<double>> costMatrix){
    return MakePermutation(source, rank, size, costMatrix);
}