#pragma once
#include "opencv2/ximgproc/segmentation.hpp"
#include <vector>

class neighboring_regions {
private:
    cv::Rect r1;
    cv::Rect r2;
    double _similarity;
public:
    neighboring_regions(cv::Rect r1, cv::Rect r2);
    ~neighboring_regions();
    
    bool operator<(const neighboring_regions &r) const;
    
    static double calculate_sim(cv::Rect r1, cv::Rect r2);
    void to_vector(std::vector<cv::Rect> &v);
};