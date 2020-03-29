//
// Created by Brady Bodily on 3/29/20.
//

#include "CostMatrixGenerator.h"
std::vector<std::vector<double>> CostMatrixGenerator::GenerateCostMatrix(std::vector<City> cities)
{
    std::vector<std::vector<double>>matrix (cities.size(), std::vector<double>(cities.size(), 0));
    for(int i = 0; i < cities.size(); i++){
        for(int j = 0; j < cities.size(); j++){
            auto xDif = (cities[i].GetX()-cities[j].GetX());
            auto yDif = (cities[i].GetY()-cities[j].GetY());
            matrix[i][j]= std::abs(std::sqrt( xDif^2 + yDif^2));
        }
    }
    return matrix;
}
