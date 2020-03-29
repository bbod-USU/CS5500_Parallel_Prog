//
// Created by Brady Bodily on 3/28/20.
//
#include <experimental/filesystem>
#include "ReadFromFile.h"

std::vector<City> ReadFromFile::ReadFile(const std::string& fileName)
{
    std::vector<City> cities;
    std::ifstream infile(fileName);
    int x, y;
    int name = 0;
    while (infile >> x >> y) {
        City city;
        city.SetCoordinates(x, y, name);
        cities.push_back(city);
        name++;
    }
    return cities;
}