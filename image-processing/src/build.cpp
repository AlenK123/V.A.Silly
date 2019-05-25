#include <unistd.h>
#include <sys/stat.h>
#include <iostream>

#include "detect_objects.hpp"

int main(int argc, char ** argv) { 
    if (argc < 3) {
        std::cerr << argv[0] << " <path> <n threads>" << std::endl;
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

    model keras_model; /* initialize the python module */
    const int n_threads = std::atoi(argv[2]);

    std::system("clear");

    /* optimizing runtime using threads */
    cv::setUseOptimized(true);
    cv::setNumThreads(n_threads);
    
    ssptr ss = createSelectiveSearchSegmentation();

    cv::VideoCapture cap(0);

    if(!cap.isOpened()) return EXIT_FAILURE;
    int i = 0;
    for(cv::Mat frame; cv::waitKey(1) != 'q'; cap >> frame)
    {   std::cout << i << std::endl;
        if(!frame.empty()){
            cv::Mat image(frame);
            classifications_t boxes = detect_objects(image, ss, keras_model, n_threads);
            cv::imshow("edges", draw_rois(image, boxes));
        }
        i++;
    }

    return EXIT_SUCCESS;
}