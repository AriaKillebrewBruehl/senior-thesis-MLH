#include <cmath>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

#include "../boiler-plate/CIEprocess.hpp"
#include "../boiler-plate/invert.hpp"
#include "../boiler-plate/read-save.hpp"
#include "../boiler-plate/type2str.hpp"
#include "sobel_operator.hpp"

// spatial weight function
int ws(cv::Vec2i x, cv::Vec2i y, int r);
float g(cv::Point);
// magnitude weight function
float wm(cv::Point x, cv::Point y, cv::Mat gHat, int eta);
// direction weight function
int wd(cv::Vec2i x, cv::Vec2i y);
int phi(cv::Point x, cv::Point y);
cv::Mat normalizedGradientMagnitude(cv::Mat m);
cv::Mat normalizeMatrix(cv::Mat m);
cv::Mat ETFFilter(cv::Mat tCurX, cv::Mat tCurY, cv::Mat gHat, int r, int eta);
cv::Mat ETF(std::string path, cv::Mat img, bool saving);