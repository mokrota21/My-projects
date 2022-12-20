#include "Color.h"

Color& Color::Normalize() {
    r_ = std::clamp(r_, 0.0, 1.0);
    g_ = std::clamp(g_, 0.0, 1.0);
    b_ = std::clamp(b_, 0.0, 1.0);
    return *this;
}

Color &Color::operator=(const double& i) {
    r_ = i;
    b_ = i;
    g_ = i;
    return *this;
}

Color &Color::operator+=(const Color &other) {
    g_ += other.g_;
    b_ += other.b_;
    r_ += other.r_;
    return *this;
}

Color Color::operator*(const double &other) const {
    Color c;
    c.g_ = g_ * other;
    c.r_ = r_ * other;
    c.b_ = b_ * other;
    return c;
}
