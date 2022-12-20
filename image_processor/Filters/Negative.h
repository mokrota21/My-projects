#pragma once
#include "Grayscale.cpp"

class Negative : public Filter {
public:
    Negative() {};
    Image UseFilter(const Image& image) const override;
    ~Negative() = default;
};
