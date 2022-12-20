#pragma once
#include "BMP.h"

void NumberToBit(int32_t n, unsigned char* res) {
    res[0] = n;
    res[1] = n >> 8;
    res[2] = n >> 16;
    res[3] = n >> 24;
}

Image ReadBMP(std::string path) {
    std::ifstream bmp;
    unsigned char header[header_size];
    unsigned char info[info_size];
    std::vector<Color> pixels;

    bmp.open(path, std::ios::in | std::ios::binary);
    if (!bmp.is_open()) {
        std::cerr << path << " Can't access file" << std::endl;
        throw std::invalid_argument("Can't access file");
    }

    bmp.read(reinterpret_cast<char*>(header), header_size);
    if (header[0] != 'B' && header[1] != 'M') {
        std::cerr << path << " File is not bmp" << std::endl;
        throw std::invalid_argument("File is not bmp");
        return Image();
    }

    bmp.read(reinterpret_cast<char*>(info), info_size);
    int width = info[4] + (info[5] << 8) + (info[6] << 16) + (info[7] << 24);
    int height = info[8] + (info[9] << 8) + (info[10] << 16) + (info[11] << 24);

    const int remain = (4 - (width * 3) % 4) % 4;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            unsigned char color[3];
            bmp.read(reinterpret_cast<char*>(color), 3);
            Color c(color[2] / max_color, color[1] / max_color, color[0] / max_color);
            pixels.push_back(c);
        }
        bmp.read(nullptr, remain);
    }
    bmp.close();
    return Image(height, width, pixels);
}

HeaderBMP::HeaderBMP(int file_size) {
    NumberToBit(file_size, file_size_);
}

void HeaderBMP::FillHeader(unsigned char* header) const {
    header[0] = 'B';
    header[1] = 'M';

    for (int i = 0; i < bits; ++i) {
        header[2 + i] = file_size_[0];
    }

    for (int i = bits + 1; i < header_size; ++i) {
        header[i] = 0;
    }
}

InfoBMP::InfoBMP(int height, int width) {
    NumberToBit(height, height_);
    NumberToBit(width, width_);
}

void InfoBMP::FillInfo(unsigned char *info) const {
    info[0] = info_size;
    info[1] = 0;
    info[2] = 0;
    info[3] = 0;

    for (int i  = 0; i < bits; ++i) {
        info[4 + i] = width_[i];
    }

    for (int i  = 0; i < bits; ++i) {
        info[8 + i] = height_[i];
    }

    info[12] = planes;
    info[13] = 0;

    info[14] = bits_per_pixel;
    info[15] = 0;

    for (int i = 16; i < info_size; ++i) {
        info[i] = 0;
    }
}

void SaveBMP(const Image& image, const std::string& path) {
    std::ofstream bmp;
    unsigned char header_char[header_size];
    unsigned char info_char[info_size];
    int width;
    int height;
    std::pair<int, int> p = image.GetHW();
    height = p.first;
    width = p.second;
    int remain = (4 - (width * 3) % 4) % 4;
    int size = remain * height + header_size + info_size + 3 * width * height;
    HeaderBMP header(size);
    header.FillHeader(header_char);
    InfoBMP info(height, width);
    info.FillInfo(info_char);

    bmp.open(path, std::ios::out | std::ios::binary);
    if (!bmp.is_open()) {
        std::cerr << "Can't save here: " << path << std::endl;
        return;
    }

    bmp.write(reinterpret_cast<char*>(header_char), header_size);
    bmp.write(reinterpret_cast<char*>(info_char), info_size);

    unsigned char remain_fill[3] = {0, 0, 0};
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            auto r = static_cast<unsigned char>(image[y * width + x].r_ * max_color);
            auto g = static_cast<unsigned char>(image[y * width + x].g_* max_color);
            auto b = static_cast<unsigned char>(image[y * width + x].b_* max_color);
            unsigned char color[3] = {b, g, r};
            bmp.write(reinterpret_cast<char*>(color), 3);
        }
        bmp.write(reinterpret_cast<char*>(remain_fill), remain);
    }
    bmp.close();
}
