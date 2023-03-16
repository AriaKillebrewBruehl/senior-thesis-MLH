#include "CIEprocess.hpp"

inline uchar reduceVal(const uchar val, int bins) {
    int bin_size = 255 / bins;
    // if (val > 255 - bin_size) {
    //     return 255;
    // } else {
    return uchar(val / bin_size) * bin_size;
    // if (val < 192) return uchar(val / 64.0 + 0.5) * 64;
    // }
}

// return a grayscale version of the image with only the L component
cv::Mat processColors(cv::Mat img, int bins) {
    assert(!img.empty());

    if (img.type() != 16) {
        if (img.channels() == 1) {
            cv::cvtColor(img, img, cv::COLOR_GRAY2RGB);
        }
        if (img.channels() == 4) {
            cv::cvtColor(img, img, cv::COLOR_RGBA2RGB);
        }
        img.convertTo(img, 16);
    }

    // cv::Mat chans[3];
    // cv::split(img, chans);
    // cv::Mat L = chans[0];
    // save(L, "", "-L");
    // cv::Mat a = chans[1];
    // save(a, "", "-a");
    // cv::Mat b = chans[2];
    // save(b, "", "-b");

    cv::Mat gs = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            uchar L = reduceVal(img.at<cv::Vec3b>(i, j)[0], bins);
            gs.at<uchar>(i, j) = (L);
        }
    }
    return gs;
}