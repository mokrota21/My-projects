#pragma once
#include "Crop.h"


Image Crop::UseFilter(const Image &image) const {
    int width;
    int height;
    std::pair<int, int> p = image.GetHW();
    height = p.first;
    width = p.second;
    std::vector<Color> new_pixels;

    for (int y = height - height_; y < height; ++y) {
        for (int x = 0; x < width_; ++x) {
            new_pixels.push_back(image[y * width + x]);
        }
    }
    return Image(height_, width_, new_pixels);
}