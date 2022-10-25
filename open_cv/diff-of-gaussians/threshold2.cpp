#include "threshold-edges.hpp"

void isolate (cv::Mat& comp, int label) {
    for (int i = 0; i < comp.rows; i++) {
        for (int j = 0; j < comp.cols; j++) {
            // if the pixel is not black or the color of the label set it to black
            if (comp.at<uchar>(cv::Point(i, j)) != 0 && comp.at<uchar>(cv::Point(i, j)) != label) {
                comp.at<uchar>(cv::Point(i, j)) = 0;
            }
        }
    }
}
bool meetsThreshold(cv::Mat img, int threshold) {
    if (img.empty()) {
        return false;
    }
    if (cv::countNonZero(img) < threshold) {
        return false;
    }
    return true;
}

void removeComponent(cv::Mat&labels, int x, int y, int w, int h,int label, std::string path) {
    std::string file_type = path.substr(path.length()-4, 4);
    std::string output_file = path + "-thresh-before" + std::to_string(i) + file_type;
    cv::imwrite(output_file, labels);
    // set 
    for (int i = y; i < y + h; i++) {
        for (int j = x; j < x + w; j++) {
            if (labels.at<uchar>(i, j) == label) {
                labels.at<uchar>(i, j) = 0;
            }
        }
    }
    // save image
    file_type = path.substr(path.length()-4, 4);
    output_file = path + "-thresh-after" + std::to_string(i) + file_type;
    cv::imwrite(output_file, labels);
}

cv::Mat threshold(std::string path, cv::Mat img, int threshold) {
    // read in image
    // image must be of type 8UC1

    cv::Mat image;
    if (img.empty() && path == "") {
        throw "Must pass in either file path, opencv image, or both";
        return image;
    }
    if (img.empty() && path != "") {
        // read in image unchanged
        image = cv::imread(path, 0);
        if (image.empty()) {
            throw "Not a valid image file.";
            return image;
        }
        if (image.type() != 0) {
            throw "Image must be of type 0 (8UC1)";
            return image;
        }
    } else if (!img.empty()) {
        image = img;
        if (image.type() != 0) {
            throw "Image must be of type 0 (8UC1)";
            return image;
        }
    }
    
    // convert to binary
    cv::threshold(image, image, 127, 255, cv::THRESH_BINARY);
    std::cout << type2str(image.type()) << std::endl;

    // get components
    cv::Mat labels;
    cv::Mat stats;
    cv::Mat centroids;
    int numComps =  cv::connectedComponentsWithStats(image, labels, stats, centroids); 
    
    // for each component 
    for(int i=0; i<stats.rows; i++) {
        int x = stats.at<int>(cv::Point(0, i));
        int y = stats.at<int>(cv::Point(1, i));
        int w = stats.at<int>(cv::Point(2, i));
        int h = stats.at<int>(cv::Point(3, i));
      
        // extract just the component from labeled image
        cv::Mat comp = labels(cv::Range(y, y+h), cv::Range(x,x+w));
        std::cout << "hi" << std::endl;
        // isolate component
        isolate(comp, i);
        std::cout << "hii" << std::endl;
        // get component skeleton 
        comp.convertTo(comp, CV_8UC1);
        cv::Mat skel = skeleton("", comp);
        std::cout << "hiii" << std::endl;

        // check if skeleton meets threshold
        bool meets = meetsThreshold(skel, threshold);
        std::cout << "hiiii" << std::endl;
        if (meets) {
            continue;
        } else {
            // remove the component from the labeled image
            removeComponent(labels, x, y, w, h, i);
            std::cout << "hiiiii" << std::endl;
        }
    }  

    // threshold labeled image 
    cv::Mat test;
    labels.convertTo(test, CV_32FC1);
    cv::threshold(test, test, 1, 255, cv::THRESH_BINARY);
    
    // save image
    if (path == "") {
        srand (time(NULL));
        int rand = std::rand() % 1000;
        path = "../images/" + std::to_string(rand) + ".png";
    }
    std::string file_type = path.substr(path.length()-4, 4);
    std::string output_file = path + "-thresh" + file_type;
    cv::imwrite(output_file, image);

    return labels;
}


int main(int argc, char** argv) {
    if (argc < 2) {
         std::cerr << "Must pass in image to run DoG on." << std::endl;
    } else {
        for (int i = 1; i < argc; i++) {
            cv::Mat image;
            threshold(argv[i], image, 10000);
        }
    }
}