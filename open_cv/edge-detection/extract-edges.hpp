#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <unordered_map>

#include "../boiler-plate/read-save.hpp"
#include "../boiler-plate/type2str.hpp"
#include "DoG.hpp"
#include "canny.hpp"
#include "threshold-edges.hpp"

cv::Mat extractEdges(std::string path, cv::Mat img, int thresh, bool saving);
cv::Mat processColors2(cv::Mat& img);
