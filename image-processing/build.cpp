#include <iostream>
#include <sys/types.h>
#include <vector>

#include "opencv2/ximgproc/segmentation.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"

#define HIGHT 256

using namespace cv::ximgproc::segmentation;
using ssptr = cv::Ptr<SelectiveSearchSegmentation>;
using rois = std::vector<cv::Rect>;

rois find_regions_of_interest(cv::Mat &image, ssptr &ss);
cv::Mat draw_rois(cv::Mat image, rois v);

int main(int argc, char ** argv) { 
    if (argc < 3) {
        printf("%s <path> <n threads>" "\n", argv[0]);
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
    rois v;
    ss->setBaseImage(image);
    ss->switchToSelectiveSearchFast();
    ss->process(v);

    /* reduce the region of intrests to relevent regions using cool math cool science */

    return v;
}

cv::Mat draw_rois(cv::Mat image, rois v) {
    cv::Mat out(image.clone());
    for(auto it : v) {
            
        rectangle(out, it, cv::Scalar(0, 255, 0));
    }
    return out;
}