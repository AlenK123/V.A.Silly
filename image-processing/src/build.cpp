#include <unistd.h>
#include <sys/stat.h>

#include <iostream>
#include <fstream>

#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "rois.hpp"
#include "model.hpp"
#include "s_except.hpp"

#define CVERRLOG "../../.log/log.cverrlog"

#define HIGHT 256

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

    model keras_model; /* initialize the python module */

    bool debug = argv[3][0] == 't';

    std::fstream err_log;
    err_log.open(CVERRLOG);

    /* optimizing runtime using threads */
    cv::setUseOptimized(true);
    cv::setNumThreads(std::atoi(argv[2]));
    
    /* 
        this function (from namespace cv::ximgproc::segmentation)
        takes a long time to run, better put it at the beginning of the main function
    */
    ssptr ss = createSelectiveSearchSegmentation();

    std::vector<bounding_box> boxes;

    cv::Mat input_im = cv::imread(argv[1]);
    /* resizing the image to a processable size */
    cv::resize(input_im, input_im, cv::Size((input_im.cols * HIGHT / input_im.rows), HIGHT));

    std::system("cat /dev/null > " CVERRLOG " && clear");

    if (debug) std::cout << "starting: ";

    /* 
     * it's always better to set the base image with the real image data rather than a reference or
     * a copy of it, it solves a lot of nasty errors that just make the algorithm not work to get all
     * of the potential regions.
     */
    ss->setBaseImage(input_im);
    
    rois R = find_regions_of_interest(input_im, ss);

    if (debug) std::cout << "number of region proposals: " << R.size() << std::endl;

    for (auto &roi : R) {
        try {
            prediction_t prediction = keras_model.predict(input_im(roi));
            if (debug) {
                std::cout << prediction.first << " : " << prediction.second << std::endl;
            }
            if (prediction.second >= (double)0.4) {
                boxes.push_back(bounding_box(roi, prediction.first));
            }
        }
        catch (s_except &e) {
            /* log program errors to console */
            std::cerr << "main: " << e.what() << std::endl;
            break;
        }
        catch (cv::Exception &e) {
            /* log opencv errors to log*/
            err_log << e.what() << std::endl;
        }
        catch (std::exception &e) {
            /* log standard exceptions to the console */
            std::cerr << "main: " << e.what() << std::endl;
            break;
        }
    }
    
    cv::imshow("output", draw_rois(input_im, boxes));
    while (cv::waitKey() != 113);

    err_log.close();
    
    return EXIT_SUCCESS;
}