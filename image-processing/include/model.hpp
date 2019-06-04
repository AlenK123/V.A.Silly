#pragma once
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "use_python.hpp"

using prediction_t = std::pair<const std::string, const double>;

class model {
private:
    module_t * tdt;
    const char * catagories[100] = { CATAGORIES };
public:
    model();
    ~model();

    prediction_t predict(cv::Mat image);   
};