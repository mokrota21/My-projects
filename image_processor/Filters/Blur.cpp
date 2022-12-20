#pragma once
#include "Blur.h"



Color GaussSum(double x, double y, double x0, double y0, double sigma, const Color& c) {
    Color res;
    double power = -((x0 - x) * (x0 - x) + (y0 - y) * (y0 - y)) / (2 * sigma * sigma);
    res = c * (1 / (2 * std::numbers::pi * sigma * sigma) * std::exp(power));
    return res;
}

Image GaussianBlur::UseFilter(const Image &image) const {
    Image copy(image);
    int height, width;
    auto p = image.GetHW();
    height = p.first;
    width = p.second;
    for (int y0 = 0; y0 < height; ++y0) {
        for (int x0 = 0; x0 < width; ++x0) {
            size_t i = y0;
            size_t j = x0;
            Color gaus_sum = {0, 0, 0};
            Matrix<Color> matrix = MatrixSizeN(x0, y0, image, static_cast<int>(6 * sigma_) + 1);
            for (size_t r = 0; r < matrix.size(); ++r) {
                Color c = matrix[r];
                int x, y;
                auto par = matrix.GetXY(r);
                x = par.first;
                y = par.second;
                gaus_sum += GaussSum(x, y, x0, y0, sigma_, c);
                gaus_sum.Normalize();
                if (gaus_sum == Color(1, 1, 1)) {
                    break;
                }
            }
            copy[i * width + j] = gaus_sum;
        }
    }
    return copy;
}
