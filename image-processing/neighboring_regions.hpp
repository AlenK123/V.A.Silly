#pragma once
#include "opencv2/ximgproc/segmentation.hpp"

class neighboring_regions {
private:
    cv::Rect r1;
    cv::Rect r2;
    double _similarity;
public:
    neighboring_regions(cv::Rect r1, cv::Rect r2);
    bool operator<(const neighboring_regions &r) const;
    bool operator>(const neighboring_regions &r) const;
    bool operator==(const neighboring_regions &r) const; 
};