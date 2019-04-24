#include "rois.hpp"

rois find_regions_of_interest(cv::Mat& image, ssptr& ss) {
    similarity_set sim_set;

    rois v;
    rois v2;
    ss->setBaseImage(image);
    ss->switchToSelectiveSearchFast();
    ss->process(v);

    /* reduce the region of intrests to relevent regions using cool math cool science */

    prepare_neighboring_rois(image, v, sim_set);

    for (auto nr : sim_set) {
        nr.to_vector(v2);
    }

    return v2;
}

cv::Mat draw_rois(cv::Mat image, rois v) {
    cv::Mat out(image.clone());

    /* drawing the regions onto the image */

    for (auto it : v) {
        cv::rectangle(out, it, cv::Scalar(0, 255, 0));
    }
    return out;
}

double analytical_distance(cv::Rect r1, cv::Rect r2) {
    return std::sqrt(std::pow(r1.x - r2.x, 2) + std::pow(r1.y - r2.y, 2) );
}

bool are_rois_neighboring(cv::Rect r_i, cv::Rect r_j) {
    /* two regions are neighboring only if their distance are within some king of threshold */
    return analytical_distance(r_i, r_j) <= NEIGHBOR_TH;
}

void prepare_neighboring_rois(const cv::Mat &image, rois v,  similarity_set &ss) {
    for (auto r1 : v) {
        for (auto r2 : v) {
            /* pushing only neighboring regions onto the set */
            if (are_rois_neighboring(r1, r2)) {
                ss.emplace(neighboring_regions(image, r1, r2));
            }
        }
    }
}
