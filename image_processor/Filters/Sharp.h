#pragma once
#include "Filter.cpp"

class Sharpening : public MatrixFilter {
private:
    const std::vector<double> matrix_ = {0, -1, 0, -1, 5, -1, 0, -1, 0};
public:
    Sharpening() = default;
    Image UseFilter(const Image& image) const override;
    ~Sharpening() = default;
};