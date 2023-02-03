#include <iostream>
#include "../boiler-plate/read-save.hpp"

struct hash_pair
{
    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2> &p) const
    {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);

        if (hash1 != hash2)
        {
            return hash1 ^ hash2;
        }
        // If hash1 == hash2, their XOR is zero.
        return hash1;
    }
};

using pixel_type = std::pair<int, int>;
using seed_map = std::unordered_map<pixel_type, std::vector<pixel_type>, hash_pair>;

cv::Mat adjust(cv::Mat img, std::string path, bool saving);