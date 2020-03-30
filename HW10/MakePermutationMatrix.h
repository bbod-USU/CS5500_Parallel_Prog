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
    static std::vector<double> GetLowestCost(std::vector<City> source, int rank, int size, std::vector<std::vector<double>> costMatrix);

  private:
    static std::vector<double> MakePermutation(std::vector<City> source, int rank, int size, std::vector<std::vector<double>> costMatrix);
};

#endif // HW10_MAKEPERMUTATIONMATRIX_H
