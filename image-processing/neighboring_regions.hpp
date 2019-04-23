#pragma once
#include "opencv2/ximgproc/segmentation.hpp"

class neighboring_regions {
private:
    std::pair<cv::Rect, cv::Rect> _regions;
    double _similarity;
public:
    neighboring_regions(cv::Rect r1, cv::Rect r2);
    bool operator<(const neighboring_regions &r1);
    //bool operator<(const neighboring_regions &r);
};