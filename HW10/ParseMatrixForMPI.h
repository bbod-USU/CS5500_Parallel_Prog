//
// Created by Brady Bodily on 3/29/20.
//
#include <vector>
#ifndef HW10_PARSEMATRIXFORMPI_H
#define HW10_PARSEMATRIXFORMPI_H
namespace matrixTools
{
    template <typename T>
    std::vector<std::vector<T>> UnflattenMatrix(std::vector<T> origianlVector, int rowSize, int columnSize)
    {
        std::vector<std::vector<T>> matrix(columnSize, std::vector<T>(rowSize, 0));
        for (int i = 0; i < columnSize; i++) {
            for (int j = 0; j < rowSize; j++) {
                matrix[i][j] = origianlVector[i + j];
            }
        }
        return matrix;
    }

    template <typename T>
    std::vector<T> FlattenMatrix(std::vector<std::vector<T>> matrix)
    {
        std::vector<T> rVector(matrix.size() * matrix[0].size(), 0);
        for (int i = 0; i < matrix.size(); i++) {
            for (int j = 0; j < matrix[0].size(); j++) {
                rVector[i + j] = matrix[i][j];
            }
        }
        return rVector;
    }
}
#endif //HW10_PARSEMATRIXFORMPI_H
