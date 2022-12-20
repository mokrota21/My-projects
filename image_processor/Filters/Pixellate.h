#pragma once
#include "Filter.cpp"

Color MedianSum(Matrix<Color> matrix);
std::vector<Color> EnlargePixels(const Image& mini_pixels, int pixel_size, int height, int width);

class Pixellate : public Filter {
private:
    int pixel_size_;
public:
    Pixellate() = default;
    Pixellate(int size) : pixel_size_(size) {};
    Image UseFilter(const Image &image) const override;
    ~Pixellate() = default;
};