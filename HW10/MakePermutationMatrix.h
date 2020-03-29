//
// Created by Brady Bodily on 3/28/20.
//

#ifndef HW10_MAKEPERMUTATIONMATRIX_H
#define HW10_MAKEPERMUTATIONMATRIX_H

#include "City.h"
#include <vector>
class MakePermutationMatrix {
public:
  static std::vector<std::vector<City>> MakePermutation(std::vector<City> source, int rank, int size);
};

#endif // HW10_MAKEPERMUTATIONMATRIX_H
