#include "neighboring_regions.hpp"


neighboring_regions::neighboring_regions(cv::Rect r1, cv::Rect r2) {
    this->r1 = r1;
    this->r2 = r2;
    this->_similarity = 0;
}

bool neighboring_regions::operator<(const neighboring_regions &r) const {
    return (this->_similarity < r._similarity);
}

bool neighboring_regions::operator>(const neighboring_regions &r) const {
    return (this->_similarity > r._similarity);
}

bool neighboring_regions::operator==(const neighboring_regions &r) const {
    return false;
}
