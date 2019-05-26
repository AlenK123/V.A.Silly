#include <unistd.h>
#include <sys/stat.h>
#include <iostream>

#include "detect_objects.hpp"

int main(int argc, char ** argv) {
    if (argc < 3) {
        std::cerr << argv[0] << "<media path> <n_threads>" << std::endl;
        return EXIT_FAILURE;
    }
    
    cv::VideoCapture * cap = nullptr;

    if (std::strcmp(argv[1], "0") == 0) {
        cap = new cv::VideoCapture(0);
    }
    else {
        /* fastest way to check if the file exists */    
        struct stat buffer;
        if (stat (argv[1], &buffer) != 0) {
            std::cout << "File " << argv[1] << " doesn\'t exist" << std::endl;
            return EXIT_FAILURE;
        }

        cap = new cv::VideoCapture(argv[1]);
    } 
    
    if(!cap->isOpened()) return EXIT_FAILURE;
    
    const int n_threads = std::atoi(argv[2]);

    model keras_model; /* initialize the python module */

    std::system("clear");

    /* optimizing runtime using threads */
    cv::setUseOptimized(true);
    cv::setNumThreads(n_threads);
    
    ssptr ss = createSelectiveSearchSegmentation();

    for(cv::Mat frame; cv::waitKey(1) != 'q'; (*cap) >> frame) {
        if(!frame.empty()) {
            cv::Mat image(frame);
            classifications_t boxes = detect_objects(image, ss, keras_model, n_threads);
            cv::imshow("edges", draw_rois(image, boxes));
        }
    }

    delete cap;
    return EXIT_SUCCESS;
}