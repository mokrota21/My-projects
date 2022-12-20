#pragma once
#include "Image.h"
#include <algorithm>

std::pair<int, int> Image::GetHW() const {
    return {height_, width_};
}

size_t ClosestPoint(int x0, int y0, int width, int height) {
    x0 = std::clamp(x0, 0, width - 1);
    y0 = std::clamp(y0, 0, height - 1);
    return y0 * width + x0;
}

Matrix<Color> MatrixSizeN(int x0, int y0, const Image& image, int n) {
    std::vector<Color> res;
    int width, height;
    auto p = image.GetHW();
    height = p.first;
    width = p.second;
    std::vector<std::pair<int, int>> xy;
    for (int y = y0 - static_cast<int>(n / 2) + (n + 1) % 2; y <= y0 + static_cast<int>(n / 2); ++y) {
        for (int x = x0 - static_cast<int>(n / 2) + (n + 1) % 2; x <= x0 + static_cast<int>(n / 2); ++x) {
            size_t pos = ClosestPoint(x, y, width, height);
            res.push_back(image[pos]);
            xy.emplace_back(x, y);
        }
    }
    return Matrix<Color>(res, xy);
}

Color &Image::At(int x, int y) {
    return matrix_[ClosestPoint(x, y, width_, height_)];
}

Color Image::At(int x, int y) const {
    return matrix_[ClosestPoint(x, y, width_, height_)];
}

