#include <unistd.h>
#include <sys/stat.h>
#include <iostream>

#include "detect_objects.hpp"

int main(int argc, char ** argv) { 
    if (argc < 4) {
        std::cerr << argv[0] << " <path> <n threads> <debug>" << std::endl;
        return EXIT_FAILURE;
    }
    
    /* fastest way to check if the file exists */
    {
        struct stat buffer;
        if (stat (argv[1], &buffer) != 0) {
            std::cout << "File " << argv[1] << " doesn\'t exist" << std::endl;
            return EXIT_FAILURE;
        }
    }

    bool debug = argv[3][0] == 't';

    model keras_model; /* initialize the python module */

    std::system("clear");

    /* optimizing runtime using threads */
    cv::setUseOptimized(true);
    cv::setNumThreads(std::atoi(argv[2]));
    
    /* 
     * this function (from namespace cv::ximgproc::segmentation)
     * takes a long time to run, better put it at the beginning of the main function.
     */
    ssptr ss = createSelectiveSearchSegmentation();

    cv::Mat image = cv::imread(argv[1]);

    if (debug) std::cout << "init1" << std::endl;

    classifications_t boxes = detect_objects(image, ss, keras_model);

    cv::imwrite("../../images/output.jpg", draw_rois(image, boxes));
    
    return EXIT_SUCCESS;
}