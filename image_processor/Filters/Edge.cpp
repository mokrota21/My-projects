#include "Edge.h"

Image Edge::UseFilter(const Image &image) const {
    Image copy(image);
    int width, height;
    auto p = image.GetHW();
    height = p.first;
    width = p.second;
    Grayscale gs;
    copy = gs.UseFilter(copy);
    copy = ApplyMatrix(copy, matrix_);
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            if (copy[y * width + x].r_ > threshold_) {
                copy[y * width + x] = Color(1, 1, 1);
            } else {
                copy[y * width + x] = Color(0, 0, 0);
            }
        }
    }
    return copy;
}