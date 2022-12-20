#pragma once
#include "Grayscale.cpp"

class Edge : public MatrixFilter {
private:
    double threshold_;
    const std::vector<double> matrix_ = {0, -1, 0, -1, 4, -1, 0, -1, 0};
public:
    Edge(const double& threshold) : threshold_(threshold) {};
    Image UseFilter(const Image& image) const override;
    ~Edge() = default;
};