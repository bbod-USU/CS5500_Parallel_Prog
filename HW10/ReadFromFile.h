//
// Created by Brady Bodily on 3/28/20.
//
#include "City.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#ifndef HW10_READFROMFILE_H
#define HW10_READFROMFILE_H

class ReadFromFile {
public:
  static std::vector<City> ReadFile(const std::string& fileName);
};

#endif // HW10_READFROMFILE_H
