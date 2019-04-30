#pragma once
#include <opencv2/ximgproc/segmentation.hpp>
#include "region_similarity.hpp"

#include <vector>

class neighboring_regions {
private:
    const cv::Mat &image;
    cv::Rect r1;
    cv::Rect r2;
    double _similarity;

    /* 
    * calculates the color similarity between to regions 
    * **(input: image, neighboring regions)**
    * **(output: general similarity)**
    */
    static double calculate_sim(const cv::Mat &image, cv::Rect r1, cv::Rect r2);
public:
    neighboring_regions(const cv::Mat &_image, cv::Rect r1, cv::Rect r2);
    ~neighboring_regions();
    /* only for sorting neighboring regions on the set where we take the most similar pair */
    bool operator<(const neighboring_regions &r) const;

    bool regards(const neighboring_regions &ins) const;
    bool regards(const cv::Rect &r) const;
    
    double get_sim() const;
    void to_vector(std::vector<cv::Rect> &v) const;
};