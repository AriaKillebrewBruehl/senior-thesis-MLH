#include <iostream>
#include <string>
#include <unordered_map>
#include <opencv2/opencv.hpp>

struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2>& p) const {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
 
        if (hash1 != hash2) {
            return hash1 ^ hash2;             
        }
        // If hash1 == hash2, their XOR is zero.
          return hash1;
    }
};

using pixel_type = std::pair<int, int>;
using seed_map = std::unordered_map<pixel_type, pixel_type, hash_pair>;  

seed_map
get_seeds(cv::Mat img) {
    // seeds[(i, j)] gives the seed pixel for (i, j)
    seed_map seeds;
    // default seed for undefined pixels
    pixel_type undef (-1, -1);
    for (int i = 0; i < img.rows; i++) {
        for(int j = 0; j < img.cols; j++) {
            pixel_type pix(i, j);
            unsigned char r = img.at<cv::Vec3b>(i, j)[0];
            unsigned char g = img.at<cv::Vec3b>(i, j)[1];
            unsigned char b = img.at<cv::Vec3b>(i, j)[2];

            // the pixel is undefined
            if ((r + g + b) == 0) {
                seeds[pix] = undef;
            } else {
                seeds[pix] = pix;
            }
        }
    }
    return seeds;
}

int 
jmp_flood(std::string img) {
    cv::Mat image;
    // read image
    image = cv::imread(img);
    if (img.empty()) {
        throw "Not a valid image file";
        return -1;
    }

    if (image.rows != image.cols) {
        std::string resz;
        std::cout << "Input image must be an N x N square." << std::endl;
        std::cout << "Would you like to resize image? [y/n] ";
        std::cin >> resz;
        // resize image to square
        if (resz == "y" || resz == "Y") {
            if (image.rows < image.cols) {
                cv::resize(image, image, cv::Size(image.rows, image.rows));
            } else {
                cv::resize(image, image, cv::Size(image.cols, image.cols));
            }
        } else {
            throw "Input image must be an N x N square.";
            return -1;
        }
    }
    seed_map seeds = get_seeds(image);

    int N = image.cols;
    int k = 1;

    while (N/k >= 1) {
        // loop over image
        for (int i = 0; i < image.cols; i++) {
            for (int j = 0; j < image.rows; j++) {
                pixel_type p(i, j);
                // extract pixel rgb values
                unsigned char pr = image.at<cv::Vec3b>(i, j)[0]; 
                unsigned char pg = image.at<cv::Vec3b>(i, j)[1]; 
                unsigned char pb = image.at<cv::Vec3b>(i, j)[2]; 
                // unsigned char pa = image.at<cv::Vec3b>(i, j)[3];

                // loop over 9 neighbors 
                std::vector<int> nbrs_i = {i - k, i, i + k};
                for (int qi : nbrs_i) {
                    // bounds check
                    if (qi < 0 || qi > N - 1) {
                        continue;
                    }
                    std::vector<int> nbrs_j = {j - k, j, j + k};
                    for (int qj : nbrs_j) {
                        // bounds check 
                        if (qj < 0 || qj > N - 1) {
                            continue;
                        } 
                        else if (i == qi && j == qj) {
                            continue;
                        }
                        // don't bother if neighbor is yourself
                        pixel_type q(qi, qj);

                        unsigned char qr = image.at<cv::Vec3b>(qi, qj)[0]; 
                        unsigned char qg = image.at<cv::Vec3b>(qi, qj)[1]; 
                        unsigned char qb = image.at<cv::Vec3b>(qi, qj)[2]; 
                        // unsigned char qa = image.at<cv::Vec3b>(qi, qj)[3];

                        // if p is colored and q is undefined continue
                        if (((pr + pg + pb) != 0) && ((qr + qg + qb) == 0)) {
                            continue;
                        }
                        // if both are undefined continue 
                        else if (((pr + pg + pb) == 0) && ((qr + qg + qb) == 0)) {
                            continue;
                        }
                        // p is undefined but q is not 
                        else if (((pr + pg + pb) == 0) && ((qr + qg + qb) != 0)) {
                            // set p to be the color of its q
                            image.at<cv::Vec3b>(i, j)[0] = qr;
                            image.at<cv::Vec3b>(i, j)[1] = qg;
                            image.at<cv::Vec3b>(i, j)[2] = qb;
                            // update pixel map
                            seeds[p] = seeds[q];
                        }
                        // both p and q are colored  
                        else if (((pr + pg + pb) != 0) && ((qr + qg + qb) != 0)) {
                            pixel_type p_seed = seeds[p];
                            pixel_type q_seed = seeds[q];

                            double dist_p_seed = std::hypot(p.first - p_seed.first, p.second - p_seed.second);
                            double dist_q_seed = std::hypot(p.first - q_seed.first, p.second - q_seed.second);
                            // p is closer to q's seed than its own
                            if (dist_p_seed > dist_q_seed) {
                                // set p to be the color of q
                                image.at<cv::Vec3b>(i, j)[0] = qr;
                                image.at<cv::Vec3b>(i, j)[1] = qg;
                                image.at<cv::Vec3b>(i, j)[2] = qb;
                                // update pixel map
                                seeds[p] = seeds[q];
                            }
                        }
                      
                    }
                }
            }
        }
        k *= 2;
    }
    // save image
    std::string file_type = img.substr(img.length()-4, 4);
    std::string output_file = img + "-jfa" + file_type;
    cv::imwrite(output_file, image);
    return 0;
}

int main(int argc, char** argv) {
    if (argc < 2) {
         std::cerr << "Must pass in image to run jfa on." << std::endl;
    } else {
        for (int i = 1; i < argc; i++) {
            jmp_flood(argv[i]);
        }
    }
}

