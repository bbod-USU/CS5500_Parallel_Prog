//
// Created by Brady Bodily on 4/13/20.
//
#include <vector>
#include<iostream>
#include <math.h>

#ifndef HW10_SERIALIZE_DESERIALIZE_VECTOR_H
#define HW10_SERIALIZE_DESERIALIZE_VECTOR_H
class Serialize_Deserialize_Vector{
public:
  static std::vector<double> Serialize(std::vector<std::vector<double> > data);
  static std::vector<std::vector<double> > Deserialize(std::vector<double> data);
};

std::vector<double> Serialize_Deserialize_Vector::Serialize(std::vector<std::vector<double> > data) {
  std::vector<double>rdata = std::vector<double>(data.size()*data.size());
  for(int i = 0; i < data.size(); i++){
    for(int j= 0; j<data.size(); j++){
      rdata[(i*data.size())+j]=data[i][j];
    }
  }
  return rdata;
}

std::vector<std::vector<double> > Serialize_Deserialize_Vector::Deserialize(std::vector<double> data) {
  std::vector<std::vector<double> > rdata = std::vector<std::vector<double> >(data.size()/2, std::vector<double>( sqrt(data.size())));
  int index = 0;
  for(int i = 0; i < sqrt(data.size()); i++){
    for(int j = 0; j<  sqrt(data.size()); j++) {
      rdata[i][j]=data[index];
      index++;
    }
  }
  return rdata;
}
#endif // HW10_SERIALIZE_DESERIALIZE_VECTOR_H
