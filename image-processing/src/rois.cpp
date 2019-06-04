#include "rois.hpp"

rois find_regions_of_interest(cv::Mat &image, ssptr &ss, int n_threads) {
    similarity_set sim_set;
    similarity_set deleted_nr;

    rois v;
    rois regions;

    ss->switchToSelectiveSearchFast();
    ss->process(v);

    /* prepare all of the neighboring regions of interest and calculate the smilarity between them */
    prepare_neighboring_rois(image, v, sim_set, n_threads);
    
    /* reduce the region of intrests to relevent regions using cool math cool science */
    while (sim_set.empty() == false) {
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
        similarity_set last = deleted_nr;
        deleted_nr = remove_instances(sim_set, s_max);


        /* this is not a bug it's a feature */
        if (deleted_nr.size() == last.size()) {
            break;
        }

        /* calculate similarity St between t and it's neighbors */
        for (auto k = deleted_nr.begin(); k != deleted_nr.end(); ++k) {
            if (k != s_max) {
                rois _v;
                k->to_vector(_v);
                cv::Rect n = _v[s_max->regards(_v[0])];
                sim_set.emplace(neighboring_regions(image, t, n));
            }
        }

        regions.push_back(t);
    }

    return regions;
}

cv::Rect rescale(cv::Mat &image, cv::Rect roi) {
    double rl = (HIGHT * image.cols / std::pow(image.rows, 2));
    return cv::Rect(roi.x / rl, roi.y / rl, 2 * roi.width / rl, 2 * roi.height / rl);
}

cv::Mat draw_rois(cv::Mat image, std::vector<bounding_box> v) {
    cv::Mat out(image.clone());

    /* drawing the regions onto the image */

    for (auto it : v) {
        cv::Rect r = rescale(image, it.first);
        cv::rectangle(out, r, cv::Scalar(0, 255, 0));
        cv::putText(
            out,
            it.second, 
            cv::Point(r.x, r.y), 
            cv::FONT_HERSHEY_SIMPLEX,
            0.7,
            cv::Scalar(0, 0, 255)
        );
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

void _prepare_neighboring_rois(const cv::Mat &image, rois::iterator b, rois::iterator e, similarity_set &ss) {
    for (auto r1 = b; r1 != e; ++r1) {
        for (auto r2 = b; r2 != e; ++r2) {
            /* pushing only neighboring regions onto the set */
            if (are_rois_neighboring(*r1, *r2)) {
                ss.emplace(neighboring_regions(image, *r1, *r2));
            }
        }
    }
}

void prepare_neighboring_rois(const cv::Mat &image, rois v, similarity_set &ss, const int n_threads) {
    int add = v.size() / n_threads;
    rois::iterator begin = v.begin();
    for (int i = 1; i <= n_threads; i++) {
        std::thread t(
            _prepare_neighboring_rois, 
            std::ref(image), begin + (i - 1) * add, 
            begin + (i * add),
            std::ref(ss)
        );
        t.join();
    }
    if ((v.size() - n_threads * add) != 0) {
        _prepare_neighboring_rois(image, begin + add * n_threads, v.end(), ss);
    }
}

similarity_set remove_instances(similarity_set &ss, std::set<neighboring_regions>::iterator ins) {
    similarity_set s;
    for (auto it = ss.begin(); it != ss.end(); ) {
        if (it->regards(*ins)) {
            s.emplace(*it);
            ss.erase(it++);
        }
        else {
            ++it;
        }
    }
    return s;
}