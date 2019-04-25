#include "rois.hpp"

rois find_regions_of_interest(cv::Mat& image, ssptr& ss) {
    similarity_set sim_set;

    rois v;
    rois R;
    ss->setBaseImage(image);
    ss->switchToSelectiveSearchFast();
    ss->process(v);

    /* prepare all of the neighboring regions of interest and calculate the smilarity between them */

    /* TODO: v is the set R in the pdf change it */

    prepare_neighboring_rois(image, v, sim_set);

    /* reduce the region of intrests to relevent regions using cool math cool science */
    while (!sim_set.empty()) {
        rois nr; //get neighboring regions
        /* get the highest similarity pair */
        auto s_max = --sim_set.end(); // last member has the heighest two regions similarity so: end() - 1 = last 
        s_max->to_vector(nr);

        /* merge corresponding regions */
        cv::Rect t(
            std::min(nr[0].x, nr[1].x), 
            std::min(nr[0].y, nr[1].y),
            std::max(nr[0].width, nr[1].width), 
            std::max(nr[0].height, nr[1].height)
        );

        /* remove similarities regarding ri, rj */
        remove_instances(sim_set, s_max);

        /* calculate similarity St between t and it's neighbors */
        
        R.push_back(t);
    }

    return R;
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

void remove_instances(similarity_set &ss, std::set<neighboring_regions>::iterator ins) {
    for (auto it = ss.begin(); it != ss.end(); ) {
        if (it->regards(*ins)) {
            ss.erase(it++);
        }
        else {
            ++it;
        }
    }
}