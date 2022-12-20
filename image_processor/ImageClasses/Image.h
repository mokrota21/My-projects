#pragma once
#include "Color.cpp"
#include "Matrix.cpp"

size_t ClosestPoint(size_t x0, size_t y0, int width, int height);
Matrix<Color> MatrixSizeN(int x0, int y0, int height, int width, int n);

class Image : public Matrix<Color> {
private:
    int height_;
    int width_;
public:
    Image() : Matrix<Color>(), height_(0), width_(0) {};
    Image(int height, int width, std::vector<Color> p) : Matrix<Color>(p, height, width), height_(height), width_(width) {};
    Image(const Image& image) = default;
    std::pair<int, int> GetHW() const;
    Color& At(int x, int y);
    Color At(int x, int y) const;
    Image& operator=(const Image& other) = default;
    ~Image() = default;
};