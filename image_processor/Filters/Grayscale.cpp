#pragma once
#include "Grayscale.h"

Image Grayscale::UseFilter(const Image &image) const {
    double gray;
    Image copy(image);
    int width, height;
    auto p = image.GetHW();
    height = p.first;
    width = p.second;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            size_t pos = y * width + x;
            gray = Product<double, double>({copy[pos].r_, copy[pos].g_, copy[pos].b_}, gray_);
            copy[pos] = Color(gray, gray, gray);
        }
    }
    return copy;
}