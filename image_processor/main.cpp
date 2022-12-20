#include "./Filters/Crop.cpp"
#include "./ImageClasses/BMP.cpp"
#include "./Filters/Grayscale.cpp"
#include "./Filters/Negative.cpp"
#include "./Filters/Sharp.cpp"
#include "./Filters/Edge.cpp"
#include "./Filters/Blur.cpp"
#include "./Filters/Pixellate.cpp"
#include <memory>

std::unique_ptr<Filter> CreateFilter(char** data, int& i) {
    Filter* res;
    if ("-crop" == static_cast<std::string>(data[i])) {
        try {
            res = new Crop(std::stoi(data[i + 1]), std::stoi(data[i + 2]));
            i += 2;
        } catch (...) {
            std::cerr << "Trouble with crop" << std::endl;
            throw std::invalid_argument("Trouble with crop");
        }
    } else if ("-gs" == static_cast<std::string>(data[i])) {
        try {
            res = new Grayscale();
        } catch (...) {
            std::cerr << "Trouble with gray" << std::endl;
            throw std::invalid_argument("Trouble with gray");
        }
    } else if ("-neg" == static_cast<std::string>(data[i])) {
        try {
            res = new Negative();
        } catch (...) {
            std::cerr << "Trouble with negative" << std::endl;
            throw std::invalid_argument("Trouble with negative");
        }
    } else if ("-sharp" == static_cast<std::string>(data[i])) {
        try {
            res = new Sharpening();
        } catch (...) {
            std::cerr << "Trouble with sharp" << std::endl;
            throw std::invalid_argument("Trouble with sharp");
        }
    } else if ("-edge" == static_cast<std::string>(data[i])) {
        try {
            res = new Edge(static_cast<double>(std::stod(data[i + 1])));
            ++i;
        } catch (...) {
            std::cerr << "Trouble with edge" << std::endl;
            throw std::invalid_argument("Trouble with edge");
        }
    } else if ("-blur" == static_cast<std::string>(data[i])) {
        try {
            res = new GaussianBlur(static_cast<double>(std::stod(data[i + 1])));
            ++i;
        } catch (...) {
            throw std::invalid_argument("Trouble with blur");
        }
    } else if ("-custom" == static_cast<std::string>(data[i])) {
        try {
            res = new Pixellate(std::stoi(data[i + 1]));
            ++i;
        } catch (...) {
            std::cerr << "Trouble with pixellate" << std::endl;
            throw std::invalid_argument("Trouble with custom filter pixellate");
        }
    } else {
        std::cerr << data[i] << " Invalid filter input" << std::endl;
        throw std::invalid_argument("Invalid filter input");
    }
    return std::unique_ptr<Filter>(res);
}

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cerr << "Invalid input";
        return 0;
    }
    Image image;
    image = ReadBMP(argv[1]);
    int i = 3;
    while (i < argc) {
        std::unique_ptr<Filter> filter(CreateFilter(argv, i));
        image = filter->UseFilter(image);
        ++i;
    }
    SaveBMP(image, argv[2]);
    return 0;
}