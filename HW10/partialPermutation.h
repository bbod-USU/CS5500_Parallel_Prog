//
// Created by Brady Bodily on 3/30/20.
//
#include <iostream>
#ifndef HW10_PARTIALPERMUTATION_H
#define HW10_PARTIALPERMUTATION_H
template<typename BidiIt>
bool next_partial_permutation(BidiIt first, BidiIt middle, BidiIt last) {
    std::reverse(middle, last);
    return std::next_permutation(first, last);
}
#endif //HW10_PARTIALPERMUTATION_H
