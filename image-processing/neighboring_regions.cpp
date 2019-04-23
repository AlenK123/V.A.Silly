#include "neighboring_regions.hpp"


neighboring_regions::neighboring_regions(cv::Rect r1, cv::Rect r2) {
    this->_regions = std::pair(r1, r2);
    this->_similarity = 0;
}

bool neighboring_regions::operator<(const neighboring_regions &r1) {
    return (this->_similarity > r1._similarity);
}
/*
bool neighboring_regions::operator>(const neighboring_regions &r) {
    return !((*this) > r);
}
*/
