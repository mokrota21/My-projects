#pragma once
#include "Filter.cpp"

class Grayscale : public Filter {
private:
    const std::vector<double> gray_ = {0.299, 0.587, 0.114};
public:
    Grayscale() {};
    Image UseFilter(const Image& image) const override;
    ~Grayscale() = default;
};
