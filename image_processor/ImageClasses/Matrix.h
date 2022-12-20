#pragma once
#include <vector>

class MatrixFilter;

template<typename T>
class Matrix {
protected:
    std::vector<T> matrix_;
    std::vector<std::pair<int, int>> xy_;
    friend MatrixFilter;
public:
    Matrix() = default;
    Matrix(const std::vector<T>& matrix, std::vector<std::pair<int, int>> xy) : matrix_(matrix), xy_(xy) {};
    Matrix(const std::vector<T>& matrix, int height, int width);
    T& operator[](size_t pos);
    T operator[](size_t pos) const;
    std::pair<int, int> GetXY(size_t pos) const;
    size_t size() const;
    auto begin();
    auto end();
    ~Matrix() = default;
};
