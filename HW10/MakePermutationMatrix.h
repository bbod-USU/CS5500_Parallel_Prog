//
// Created by Brady Bodily on 3/28/20.
//

#ifndef HW10_MAKEPERMUTATIONMATRIX_H
#define HW10_MAKEPERMUTATIONMATRIX_H

#include "City.h"
#include <vector>
class MakePermutationMatrix
{
  public:
    long double GetLowestCost(std::vector<City> source, int rank, int size, std::vector<std::vector<double>> costMatrix);

  private:
    long double MakePermutation(std::vector<City> source, int rank, int size, std::vector<std::vector<double>> costMatrix);
};

#endif // HW10_MAKEPERMUTATIONMATRIX_H
