#include "tests.hpp"

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Must pass in image to run dot placement on." << std::endl;
    } else {
        for (int i = 1; i < argc; i += 2) {
            // cv::Mat image;
            // placeDots(argv[i], image, argv[i + 1], image, true);
        }
    }
}