#pragma once
#include "Filter.cpp"

class Crop : public Filter {
private:
    int width_;
    int height_;
public:
    Crop(const int p1, const int p2) : Filter(), width_(p1), height_(p2) {};
    Image UseFilter(const Image& image) const override;
    ~Crop() = default;
};