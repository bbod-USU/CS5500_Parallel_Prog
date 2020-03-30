//
// Created by Brady Bodily on 3/30/20.
//

#include <vector>
#include "City.h"

#ifndef HW10_SERIALIZECITIES_H
#define HW10_SERIALIZECITIES_H

class SerializeCities
{
  public:
    static std::vector<double> Serialize(std::vector<City> cities);
    static std::vector<City> Deserialize(std::vector<double> cities);
};
std::vector<double> SerializeCities::Serialize(std::vector<City> cities)
{
    auto returnVector = std::vector<double>(cities.size()*3, 0);
    returnVector[0] = cities[0].GetName();
    returnVector[1] = cities[0].GetX();
    returnVector[2] = cities[0].GetY();
    for(int i = 3; i < cities.size()*3; i+=3){
        returnVector[i] = cities[i].GetName();
        returnVector[i+1] = cities[i].GetX();
        returnVector[i+2] = cities[i].GetY();

    }
    return returnVector;
}

std::vector<City> SerializeCities::Deserialize(std::vector<double> cities)
{
    auto returnVector = std::vector<City>();
    for(int i = 0; i < cities.size(); i+=3){
        City city;
        city.SetCoordinates(cities[i+1], cities[i+2], cities[i]);
        returnVector.push_back(city);
    }
    return returnVector;
}

#endif //HW10_SERIALIZECITIES_H
