#include <iostream>
#include <sys/types.h>
#include <vector>
#include <set>
#include <functional>

#include <ctime>
#include <chrono>

#include "opencv2/ximgproc/segmentation.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"

#include "neighboring_regions.hpp"

#define HIGHT 256
#define NEIGHBOR_TH (double)70

using namespace cv::ximgproc::segmentation;
using ssptr = cv::Ptr<SelectiveSearchSegmentation>;
using rois = std::vector<cv::Rect>;
using similarity_set = std::set<neighboring_regions>;


void prepare_neighboring_rois(rois v, similarity_set &ss);
double analytical_distance(cv::Rect r1, cv::Rect r2);
bool are_rois_neighboring(cv::Rect r_i, cv::Rect r_j);
rois find_regions_of_interest(cv::Mat &image, ssptr &ss);
cv::Mat draw_rois(cv::Mat image, rois v);


int main(int argc, char ** argv) { 
    if (argc < 3) {
        std::cerr << argv[0] << " <path> <n threads>" << std::endl;
        return EXIT_FAILURE;
    }

    cv::setUseOptimized(true);
    
    cv::setNumThreads(std::atoi(argv[2]));

    cv::Mat input_im = cv::imread(argv[1]);

    cv::resize(input_im, input_im, cv::Size((input_im.cols * HIGHT / input_im.rows), HIGHT));

    ssptr ss = createSelectiveSearchSegmentation();

    while (true) {
        cv::imshow("output", draw_rois(input_im, find_regions_of_interest(input_im, ss)));
        if (cv::waitKey() == 113) break;
    }
    return EXIT_SUCCESS;
}

rois find_regions_of_interest(cv::Mat& image, ssptr& ss) {
    clock_t t = clock();
    similarity_set sim_set;

    rois v;
    rois v2;
    ss->setBaseImage(image);
    ss->switchToSelectiveSearchFast();
    ss->process(v);

    std::cout << "Time elapsed: " << ((double)(clock() - t) / CLOCKS_PER_SEC) << std::endl;

    /* reduce the region of intrests to relevent regions using cool math cool science */

    prepare_neighboring_rois(v, sim_set);

    std::cout << "len: " << sim_set.size() << std::endl;

    for (auto nr : sim_set) {
        nr.to_vector(v2);
    }

    return v2;
}

cv::Mat draw_rois(cv::Mat image, rois v) {
    cv::Mat out(image.clone());
    for (auto it : v) {
        cv::rectangle(out, it, cv::Scalar(0, 255, 0));
    }
    return out;
}

double analytical_distance(cv::Rect r1, cv::Rect r2) {
    return std::sqrt( std::pow(r1.x - r2.x, 2) + std::pow(r1.y - r2.y, 2) );
}

bool are_rois_neighboring(cv::Rect r_i, cv::Rect r_j) { 
    return analytical_distance(r_i, r_j) <= NEIGHBOR_TH;
}

void prepare_neighboring_rois(rois v, similarity_set &ss) {
    for (auto r1 : v) {
        for (auto r2 : v) {
            if (are_rois_neighboring(r1, r2)) {
                ss.emplace(neighboring_regions(r1, r2));
            }
        }
    }
}
