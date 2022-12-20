#pragma once
#include "Filter.cpp"

Color GaussSum(double x, double y, double x0, double y0, double sigma, const Color& c);

class GaussianBlur : public Filter {
private:
    const double sigma_;
public:
    GaussianBlur(const double& sigma) : sigma_(sigma) {};
    Image UseFilter(const Image& image) const override;
    ~GaussianBlur() = default;
};