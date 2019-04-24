#include "neighboring_regions.hpp"

neighboring_regions::neighboring_regions(const cv::Mat &_image, cv::Rect r1, cv::Rect r2) : image(_image) {
    this->r1 = r1;
    this->r2 = r2;
    this->_similarity = neighboring_regions::calculate_sim(this->image, r1, r2);
}

neighboring_regions::~neighboring_regions() {
    //
}

bool neighboring_regions::operator<(const neighboring_regions &r) const {
    return (this->_similarity < r._similarity);
}

double neighboring_regions::calculate_sim(const cv::Mat &image, cv::Rect r1, cv::Rect r2) {
    return color_similarity(image, r1, r2);
}

double neighboring_regions::get_sim() const {
    return this->_similarity;
}

void neighboring_regions::to_vector(std::vector<cv::Rect> &v) {
    /* adding the regions onto a vector */
    v.push_back(this->r1);
    v.push_back(this->r2);
}
