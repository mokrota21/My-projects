#pragma once
#include "Image.cpp"
#include <fstream>

const int bits = 4;
const int header_size = 14;
const int info_size = 40;
const double max_color = 255.0f;
const int bits_per_pixel = 24;
const int planes = 1;

void NumberToBit(int n, unsigned char* res);
void SaveBMP(const Image& image, const std::string& path);
Image ReadBMP(std::string path);


class HeaderBMP {
private:
    unsigned char file_size_[bits];
public:
    HeaderBMP(int file_size_);
    void FillHeader(unsigned char* header) const;
    ~HeaderBMP() = default;
};

class InfoBMP {
private:
    unsigned char height_[bits];
    unsigned char width_[bits];
public:
    InfoBMP(int height, int width);
    void FillInfo(unsigned char* info) const;
    ~InfoBMP() = default;
};