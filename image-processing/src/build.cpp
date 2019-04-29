#include <iostream>
#include <vector>
#include <set>

#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"

#include "rois.hpp"

#define HIGHT 256

int main(int argc, char ** argv) { 
    if (argc < 4) {
        std::cerr << argv[0] << " <path> <n threads> <debug>" << std::endl;
        return EXIT_FAILURE;
    }
    
    bool debug = strcmp(argv[3], "t") == 0;
    /* optimizing runtime using threads */
    cv::setUseOptimized(true);
    
    cv::setNumThreads(std::atoi(argv[2]));
    /* 
        this function (from namespace cv::ximgproc::segmentation)
        takes a long time to run, better put it at the beginning of the main function
    */
    ssptr ss = createSelectiveSearchSegmentation();

    cv::Mat input_im = cv::imread(argv[1]);
    /* resizing the image to a processable size */
    cv::resize(input_im, input_im, cv::Size((input_im.cols * HIGHT / input_im.rows), HIGHT));

    if (debug) std::cout << "starting: ";
    rois R = find_regions_of_interest(input_im, ss);
    if (debug) std::cout << "number of region proposals: " << R.size() << std::endl;

    cv::imshow("output", draw_rois(input_im, R));
    while (cv::waitKey() != 113);
    
    return EXIT_SUCCESS;
}

