//
// Created by Brady Bodily on 3/28/20.
//

#include "MakePermutationMatrix.h"
#include "mpi.h"
#include <algorithm>
#include <iostream>
#include <cfloat>

long double MakePermutationMatrix::MakePermutation(std::vector<City> source, int rank, int size, std::vector<std::vector<double>> costMatrix){
    std::vector<double> returnVector;
    std::vector<std::size_t> localCostVector;
    int vectorOffset = source.size()/size;
    std::sort(source.begin(), source.end());
    std::vector<City> myPerm = source;
    std::vector<City> nextPerm = source;
    auto lowest = LDBL_MAX;

    //return vector should be the size of number of processors as they are going to only return their own smallest.
    returnVector.resize(source.size(), 0);

    //the "nextPerm" is the permutation to stop at.
    if(rank != size-1)
        std::rotate(nextPerm.begin(), nextPerm.begin() + ((rank*vectorOffset)+vectorOffset), nextPerm.end());

    std::partial_sort(nextPerm.begin()+1, nextPerm.end(), nextPerm.end());
    //rotate my perm.
    if(rank!=0)
        std::rotate(myPerm.begin(), myPerm.begin()+(((rank-1)*vectorOffset)+vectorOffset),myPerm.end());
    std::partial_sort(myPerm.begin()+1, myPerm.end(), myPerm.end());
    if(rank == size-1){
        nextPerm = source;
    }
    std::cout << "Rank: " << rank << " I'm here" << std::endl;
    do {
        double cost = 0;
        //Add up the cost of the current permutation.
        // std::cout <<"Rank: " << rank << " Current perm: ";
        //std::cout << "Rank: "<< rank;
        for (int i = 0; i < myPerm.size(); i++) {
           //std::cout << " " << myPerm[i].GetName();
            cost += costMatrix[myPerm[i].GetName()][myPerm[i-1].GetName()];
        }
        //std::cout << std::endl << std::endl;
        if(cost < lowest){
            lowest = cost;
            std::cout << "Current lowest cost: " << lowest << std::endl;
        }
        //Push permutation cost to the localCostVector.
        //localCostVector.push_back(cost);
    } while (std::next_permutation(myPerm.begin(), myPerm.end()) && myPerm!=nextPerm);

    return lowest;
}

long double MakePermutationMatrix::GetLowestCost(std::vector<City> source, int rank, int size, std::vector<std::vector<double>> costMatrix){
    std::vector<long double> l(size,0);
    auto lowest = MakePermutation(source, rank, size, costMatrix);
    MPI_Gather(&lowest,1,MPI_LONG_DOUBLE,l.data(),1,MPI_LONG_DOUBLE,0,MPI_COMM_WORLD);
    std::sort(l.begin(), l.end());
    return l[l.size()-1];
}