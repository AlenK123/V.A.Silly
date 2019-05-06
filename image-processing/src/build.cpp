#include <iostream>
#include <fstream>
#include <vector>
#include <set>

#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "rois.hpp"
#include "use_python.hpp"

#define CVERRLOG "../../.log/log.cverrlog"

#define HIGHT 256

int main(int argc, char ** argv) { 
    if (argc < 4) {
        std::cerr << argv[0] << " <path> <n threads> <debug>" << std::endl;
        return EXIT_FAILURE;
    }
    
    bool debug = argv[3][0] == 't';

    /* optimizing runtime using threads */
    cv::setUseOptimized(true);
    cv::setNumThreads(std::atoi(argv[2]));
    
    /* 
        this function (from namespace cv::ximgproc::segmentation)
        takes a long time to run, better put it at the beginning of the main function
    */
    ssptr ss = createSelectiveSearchSegmentation();

    std::fstream err_log;
    err_log.open(CVERRLOG);

    rois prediction_rois;

    to_delete_t * tdt = py_init();


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

    for (auto roi : R) {
        PyObject * prediction = NULL;
        try {
            prediction = predict(tdt, input_im(roi));
            if (debug) {
                std::cout << py_obj_to_string(prediction) << std::endl;
            }
            Py_XDECREF(prediction);
            prediction_rois.push_back(roi);
        }
        catch (cv::Exception &e) {
            /* log opencv errors to log*/
            err_log << e.what() << std::endl;
        }
        catch (std::exception &e) {
            /* log standard exceptions to the console */
            std::cerr << e.what() << std::endl;
            break;
        }
    }
    
    cv::imshow("output", draw_rois(input_im, prediction_rois));
    while (cv::waitKey() != 113);
    
    py_fin(tdt);

    err_log.close();
    
    return EXIT_SUCCESS;
}

