#pragma once
#include "Pixellate.h"

Color MedianSum(Matrix<Color> matrix) {
    Color res{};
    for (const Color& c : matrix) {
        res += c;
    }
    res = res * (1 / static_cast<double>(matrix.size()));
    res.Normalize();
    return res;
}

std::vector<Color> EnlargePixels(const Image& mini_image, int pixel_size, int height, int width) {
    std::vector<Color> res;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int x0 = static_cast<int>(x / pixel_size);
            int y0 = static_cast<int>(y / pixel_size);
            res.push_back(mini_image.At(x0, y0));
        }
    }
    return res;
}

Image Pixellate::UseFilter(const Image &image) const {
    std::vector<Color> new_pixels;
    auto p = image.GetHW();
    int width, height;
    height = p.first;
    width = p.second;
    std::vector<Color> small_pixels;

    for (int y = static_cast<int>(pixel_size_ / 2) - (pixel_size_ + 1) % 2; y < height + (pixel_size_ - height % pixel_size_) % pixel_size_; y += pixel_size_) {
        for (int x = static_cast<int>(pixel_size_ / 2) - (pixel_size_ + 1) % 2; x < width + (pixel_size_ - height % pixel_size_) % pixel_size_; x += pixel_size_) {
            auto m = MatrixSizeN(x, y, image, pixel_size_);
            small_pixels.push_back(MedianSum(m));
        }
    }

    int small_width = static_cast<int>(width / pixel_size_) + std::min(1, width % pixel_size_);
    int small_height = static_cast<int>(height / pixel_size_) + std::min(1, height % pixel_size_);
    Image small_image(small_height, small_width, small_pixels);

    new_pixels = EnlargePixels(small_image, pixel_size_, height, width);

    return Image(height, width, new_pixels);
}
