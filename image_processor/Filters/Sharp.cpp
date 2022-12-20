#pragma once
#include "Sharp.h"

Image Sharpening::UseFilter(const Image &image) const {
    return ApplyMatrix(image, matrix_);
}
