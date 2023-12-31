#include "sampled-transform-seeds.hpp"

cv::Mat3i get_seeds(cv::Mat img) {
    assert(!img.empty());
    assert(img.channels() == 1);
    if (img.type() != 4) {
        if (img.channels() != 1) {
            cv::cvtColor(img, img, cv::COLOR_RGB2GRAY);
        }
        img.convertTo(img, 4);
    }

    cv::Mat3i map = cv::Mat::zeros(img.rows, img.cols, CV_32SC3);

    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            int32_t p = img.at<int32_t>(i, j);
            map.at<cv::Vec3i>(i, j)[0] = p;
            // the pixel is not set
            if (p != 0) {
                map.at<cv::Vec3i>(i, j)[1] = -1;
                map.at<cv::Vec3i>(i, j)[2] = -1;
            } else {
                map.at<cv::Vec3i>(i, j)[1] = i;
                map.at<cv::Vec3i>(i, j)[2] = j;
            }
        }
    }
    return map;
}

cv::Mat1i get_distance(cv::Mat3i img) {
    assert(!img.empty());
    cv::Mat1i distances = cv::Mat::zeros(img.rows, img.cols, CV_32SC1);
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            distances.at<int32_t>(i, j) =
                round(sqrt(img.at<cv::Vec3i>(i, j)[0]));
        }
    }
    return distances;
}

// indicator function for membership in a set of seed pixels
int32_t f2(cv::Mat3i arr, int32_t p) {
    assert(!arr.empty());
    if (arr.type() != 20) {
        arr.convertTo(arr, 20);
    }

    try {
        if (arr.cols != 1) {
            throw arr.cols;
        }
    } catch (int i) {
        std::cout << "ERROR: Input matrix in DTOneDim has " << i << " columns. "
                  << "Must be a single column matrix." << std::endl;
        return INT32_MAX;
    }

    int32_t value = int32_t(arr.at<int32_t>(p, 0)) == 255
                        ? INT_MAX
                        : int32_t(arr.at<int32_t>(p, 0));

    return value;
}

cv::Mat3i OneD(cv::Mat3i arr, std::function<int32_t(cv::Mat, int32_t)> f) {
    // type checking
    assert(!arr.empty());

    if (arr.type() != 20) {
        arr.convertTo(arr, 20);
    }

    try {
        if (arr.rows != 1 && arr.cols != 1) {
            throw 0;
        }
    } catch (int i) {
        std::cout << "ERROR: Input matrix in OneD has dimensions " << arr.rows
                  << " x " << arr.cols
                  << ". Must be a single row or single column matrix."
                  << std::endl;
        return arr;
    }

    bool rotated = false;
    // rotate matrix so it is single column;
    if (arr.cols != 1) {
        cv::rotate(arr, arr, cv::ROTATE_90_CLOCKWISE);
        rotated = true;
    }
    assert(arr.cols == 1);

    cv::Mat final =
        cv::Mat::zeros(arr.rows, arr.cols, CV_32SC3);  // output matrix
    assert(final.type() == 20);
    int k = 0;  // index of right-most parabola in lower envelope
    std::vector<int> v{
        0};  // v[i] gives the horizontal position of the ith parabola aka our
             // position in our single column matrix
    std::vector<int> z{
        INT_MIN,
        INT_MAX};  // range in which the ith parabola of the lower envelope is
                   // below the others is given by z[i] and z[i+1]

    bool one_set = false;
    if (f2(arr, 0) != INT32_MAX) {
        one_set = true;
    }

    for (int i = 1; i < arr.rows; i++) {
        int s;
        bool curr_inf = false;
        while (true) {
            int r = v[k];
            // current pixel is neither a seed pixel nor assigned a value
            if (f2(arr, i) == INT32_MAX) {
                curr_inf = true;
                break;
            }
            // we know at least one pixel in the array has been set
            one_set = true;
            // intersection of parabola from i and r
            s = ((f2(arr, i) + (i * i)) - (f2(arr, r) + (r * r))) /
                (2 * i - 2 * r);

            if (s > z[k]) {
                break;
            }
            // if s <= z[k] then parabola from v[k] does not need to be part of
            // the lower envelope, so delete it by decreasing k
            v.erase(v.begin() + k);
            assert(!v.empty());
            z.erase(z.begin() + k);
            assert(!z.empty());
            k--;
        }
        // if the current parabola would be offset by infinity, don't add it and
        // don't change the existing lower envelope
        if (curr_inf) {
            continue;
        }
        // otherwise modify lower envelope
        // increase k
        k++;
        // add i as the kth parabola
        v.push_back(i);
        // make i is below the others starting at s
        z[k] = s;
        // and ending at infinity
        z.push_back(INT_MAX);
    }

    k = 0;
    for (int i = 0; i < arr.rows; i++) {
        // if there were no seed pixels in the array make everything int_max
        if (v.size() == 1 && !one_set) {
            final.at<int32_t>(i, 0) = 255;
            continue;
        }

        // while the range that the kth parabola covers is less than i increase
        // i
        while (z[k + 1] < i) {
            k++;
        }

        // distance between i and the horizontal position of the kth parabola
        int32_t a = abs(i - v[k]);
        int32_t b = f2(arr, v[k]);
        int32_t value = a * a + b;

        // set the distance
        final.at<cv::Vec3i>(i, 0)[0] = value;
        // set the seed pixels
        final.at<cv::Vec3i>(i, 0)[1] = arr.at<cv::Vec3i>(v[k], 0)[1];
        final.at<cv::Vec3i>(i, 0)[2] = arr.at<cv::Vec3i>(v[k], 0)[2];
    }

    if (rotated) {
        cv::rotate(final, final, cv::ROTATE_90_COUNTERCLOCKWISE);
    }
    assert(final.type() == 20);
    return final;
}

cv::Mat3i TwoD(cv::Mat3i arr, std::function<int32_t(cv::Mat, int32_t)> f) {
    // type check
    assert(!arr.empty());

    if (arr.type() != 20) {
        arr.convertTo(arr, 20);
    }

    for (int j = 0; j < arr.cols; j++) {
        // extract column and run one-dimensional distance transform
        cv::Mat3i column = arr.col(j);
        assert(column.type() == 20);
        cv::Mat3i transformed = OneD(column, f2);
        assert(column.type() == 20);
        // replace column in original array
        transformed.col(0).copyTo(arr.col(j));
    }

    for (int i = 0; i < arr.rows; i++) {
        // extract row and run one-dimensional distance transform
        cv::Mat row = arr.row(i);
        assert(row.type() == 20);
        cv::Mat3i transformed = OneD(row, f2);
        assert(row.type() == 20);
        // replace row in original array
        transformed.row(0).copyTo(arr.row(i));
    }

    return arr;
}

cv::Mat sample_seeds(cv::Mat img, std::string path, bool saving, bool seeds) {
    // read images and resize
    cv::Mat image;
    image = read(path, img);
    cv::Mat3i final;

    assert(!image.empty());
    if (image.type() != 4) {
        if (image.channels() != 1) {
            cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);
        }
        image.convertTo(image, 4);
    }
    cv::Mat inverted = invert(image);
    cv::Mat3i init_map = get_seeds(inverted);

    cv::Mat3i sampled = TwoD(init_map, f2);

    cv::Mat1i distance_only = get_distance(sampled);

    if (saving) {
        save(distance_only, path, "-sampled");
    }

    if (seeds) {
        return sampled;
    }
    return distance_only;
}