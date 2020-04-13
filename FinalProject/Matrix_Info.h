//
// Created by Brady Bodily on 4/11/20.
//

#ifndef FINALPROJECT_MATRIX_INFO_H
#define FINALPROJECT_MATRIX_INFO_H

class Matrix_Info {
  static int M, N;
  double Matrix[0][0];
public:
  Matrix_Info(int m, int n){
    M = m;
    N = n;
    Matrix = Matrix[M][N];
  }


};

#endif // FINALPROJECT_MATRIX_INFO_H
