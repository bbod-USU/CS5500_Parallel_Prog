//
// Created by Brady Bodily on 3/28/20.
//

#include "MakePermutationMatrix.h"

std::vector<std::vector<City>> MakePermutationMatrix::MakePermutation(std::vector<City> source, int rank, int size){
    std::vector<std::vector<City>> permutations;
    std::sort(source.begin(), source.end());
    std::vector<City> myPerm = source;
    std::vector<City> nextPerm = source;
    auto offset = myPerm.size()/size;

    if(!rank)
        std::rotate(nextPerm.begin(), nextPerm.begin()+offset*(rank+1), nextPerm.end());
    else
        nextPerm = source;

    if(!rank)
        std::rotate(myPerm.begin(), myPerm.begin()+offset*rank,myPerm.end());

    do {
        std::vector<City> currentPerm;
        for (int i = 0; i < myPerm.size(); i++) {
            currentPerm.push_back(myPerm[i]);
        }
        permutations.push_back(currentPerm);
    } while (std::next_permutation(myPerm.begin(), myPerm.end()), myPerm != nextPerm);
    return permutations;
}

std::vector<std::vector<City>> MakePermutationMatrix::GetLowestCost(std::vector<City> source, int rank, int size, std::vector<std::vector<double>> costMatrix){

}