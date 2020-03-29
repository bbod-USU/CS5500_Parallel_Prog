//
// Created by Brady Bodily on 3/28/20.
//

#ifndef HW10_CITY_H
#define HW10_CITY_H

class City {
  int X, Y, Name;

public:
  City() {}
  void SetCoordinates(int x, int y, int name){ X = x; Y = y; Name = name; };
  int GetX(){ return X; }
  int GetY(){ return Y; }
  int GetName(){ return Name; }
  bool operator < (const City &other) const {
    return this->Name < other.Name;
  }
  bool operator > (const City &other) const {
        return this->Name > other.Name;
  }
  bool operator == (const City &other) const{
      return this->Name == other.Name;
  }
};

#endif // HW10_CITY_H
