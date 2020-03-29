//
// Created by Brady Bodily on 3/29/20.
//

#ifndef HW10_COSTMATRIXGENERATOR_H
#define HW10_COSTMATRIXGENERATOR_H

#include "City.h"
#include <cmath>
#include <vector>
class CostMatrixGenerator
{
  public:
    static std::vector<std::vector<double>> GenerateCostMatrix(std::vector<City> cities);
};

#endif //HW10_COSTMATRIXGENERATOR_H
