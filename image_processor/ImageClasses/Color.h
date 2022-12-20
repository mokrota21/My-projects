#pragma once
#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <cmath>

class Color {
public:
    double g_;
    double b_;
    double r_;
    Color() = default;
    Color(double r, double g, double b) : r_(r), g_(g), b_(b) {};
    Color(const Color& other) = default;
    explicit Color(double i) : r_(0), g_(0), b_(0) {};
    Color& Normalize();
    Color& operator=(const Color& other) = default;
    Color& operator=(const double& i);
    bool operator==(const Color& other) const = default;
    Color& operator+=(const Color& other);
    Color operator*(const double& other) const;
};