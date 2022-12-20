#pragma once
#include "Filter.h"

template<typename T1, typename T2>
auto Product(std::vector<T1> c1, std::vector<T2> c2) {
    if (c1.size() != c2.size()) {
        throw std::invalid_argument("non equal length");
    } else {
        T1 res;
        res = 0;
        for (size_t i = 0; i < c1.size(); ++i) {
            res += c1[i] * c2[i];
        }
        return res;
    }
}

Image Filter::UseFilter(const Image &image) const {
    return image;
}

Image MatrixFilter::ApplyMatrix(const Image &image, const std::vector<double>& matrix) const {
    int n = static_cast<int>(sqrt(matrix.size()));
    Image copy(image);
    int width, height;
    auto p = copy.GetHW();
    height = p.first;
    width = p.second;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Matrix<Color> matrix1 = MatrixSizeN(x, y, image, n);
            copy[y * width + x] = Product<Color, double>(matrix1.matrix_, matrix).Normalize();
        }
    }
    return copy;
}