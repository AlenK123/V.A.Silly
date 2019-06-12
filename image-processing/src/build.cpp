#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <iostream>

#include "detect_objects.hpp"

#define ERRLOG "../../.log/errlog"

int main(int argc, char ** argv) {
    if (argc < 3) {
        std::cerr << argv[0] << "<media path> <n_threads>" << std::endl;
        return EXIT_FAILURE;
    }
    
    int _errfd = 0;

    if ((_errfd = open(ERRLOG, O_WRONLY)) < 0) {
        std::cerr << "FATAL: NO LOG FILE" << std::endl;
        return EXIT_FAILURE;
    }

    /* hook :) */
    dup2(_errfd, STDERR_FILENO);

    cv::VideoCapture * cap = nullptr;

    if (std::strcmp(argv[1], "0") == 0) {
        cap = new cv::VideoCapture(0);
    }
    else {
        /* fastest way to check if the file exists */    
        struct stat buffer;
        if (stat (argv[1], &buffer) != 0) {
            std::cerr << "File " << argv[1] << " doesn\'t exist" << std::endl;
            return EXIT_FAILURE;
        }

        cap = new cv::VideoCapture(argv[1]);
    } 
    
    if (cap->isOpened() == false) return EXIT_FAILURE;

    const int n_threads = std::atoi(argv[2]);

    model keras_model; /* initialize the python module */

    /* hook :) */
    dup2(_errfd, STDERR_FILENO);

    /* optimizing runtime using threads */
    cv::setUseOptimized(true);
    cv::setNumThreads(n_threads);
    
    ssptr ss = createSelectiveSearchSegmentation();
    
    uchar i = 0;
    for (cv::Mat frame; cv::waitKey(1) != 0; (*cap) >> frame, i++) {
        if ( frame.empty() == false && (i % 10 == 0) ) {
            cv::Mat image(frame);
            i = 0;
            classifications_t boxes = detect_objects(image, ss, keras_model, n_threads);
            cv::imshow("out", draw_rois(image, boxes));
        }
    }
    
    close(_errfd);
    delete cap;
    return EXIT_SUCCESS;
}