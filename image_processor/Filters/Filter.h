#pragma once
#include "../ImageClasses/Image.cpp"

template<typename T1, typename T2>
auto Product(std::vector<T1> c1, std::vector<T2> c2);

class Filter {
public:
    Filter() {};
    virtual Image UseFilter(const Image& image) const;
    virtual ~Filter() = default;
};

class MatrixFilter : public Filter {
public:
    Image ApplyMatrix(const Image& image, const std::vector<double>& matrix) const;
    ~MatrixFilter() = default;
};