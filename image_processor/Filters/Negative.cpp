#pragma once
#include "Negative.h"

Image Negative::UseFilter(const Image &image) const {
    Image copy(image);
    int width, height;
    auto p = image.GetHW();
    width = p.second;
    height = p.first;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            size_t pos = y * width + x;
            copy[pos].r_ = 1 - copy[pos].r_;
            copy[pos].b_ = 1 - copy[pos].b_;
            copy[pos].g_ = 1 - copy[pos].g_;
        }
    }
    return copy;
}
