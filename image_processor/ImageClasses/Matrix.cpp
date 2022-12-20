#pragma once
#include "Matrix.h"

template<typename T>
T& Matrix<T>::operator[](size_t pos) {
    return matrix_[pos];
}

template<typename T>
T Matrix<T>::operator[](size_t pos) const {
    return matrix_[pos];
}

template<typename T>
auto Matrix<T>::begin() {
    return matrix_.begin();
}

template<typename T>
auto Matrix<T>::end() {
    return matrix_.end();
}

template<typename T>
std::pair<int, int> Matrix<T>::GetXY(size_t pos) const {
    return xy_[pos];
}

template<typename T>
size_t Matrix<T>::size() const {
    return matrix_.size();
}

template<typename T>
Matrix<T>::Matrix(const std::vector<T>& matrix, int height, int width) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            matrix_.push_back(matrix[y * width + x]);
            xy_.emplace_back(x, y);
        }
    }
}