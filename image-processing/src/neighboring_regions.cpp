#include "neighboring_regions.hpp"

neighboring_regions::neighboring_regions(cv::Rect r1, cv::Rect r2) {
    this->r1 = r1;
    this->r2 = r2;
    this->_similarity = neighboring_regions::calculate_sim(r1, r2);
}

neighboring_regions::~neighboring_regions() {
    //
}

bool neighboring_regions::operator<(const neighboring_regions &r) const {
    return (this->_similarity < r._similarity);
}

double neighboring_regions::calculate_sim(cv::Rect r1, cv::Rect r2) {
    return 0.0;
}

void neighboring_regions::to_vector(std::vector<cv::Rect> &v) {
    v.push_back(this->r1);
    v.push_back(this->r2);
}
