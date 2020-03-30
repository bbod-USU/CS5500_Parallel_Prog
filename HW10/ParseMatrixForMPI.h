//
// Created by Brady Bodily on 3/29/20.
//
#include <vector>
#ifndef HW10_PARSEMATRIXFORMPI_H
#define HW10_PARSEMATRIXFORMPI_H
namespace matrixTools
{
    template <typename T>
    std::vector<std::vector<T>> UnflattenMatrix(std::vector<T> originalVector, int rowSize, int columnSize)
    {
        std::vector<std::vector<T>> matrix(columnSize, std::vector<T>(rowSize, 0));
        for (int i = 0; i < columnSize; i++) {
            for (int j = 0; j < rowSize; j++) {
                matrix[i][j] = originalVector[i + j];
            }
        }
        return matrix;
    }

    template <typename T>
    std::vector<T> FlattenMatrix(std::vector<std::vector<T>> matrix, int columnSize, int rowSize)
    {
        std::vector<T> rVector((rowSize*columnSize), 0);
        for (int i = 0; i < columnSize; i++) {
            for (int j = 0; j < rowSize; j++) {
                rVector[j*i] = matrix[i][j];
            }
        }
        return rVector;
    }
}
#endif //HW10_PARSEMATRIXFORMPI_H
