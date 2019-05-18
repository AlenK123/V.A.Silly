#include "model.hpp"

model::model() {
    this->tdt = py_init();
}

model::~model() {
    py_fin(this->tdt);
}

prediction_t model::predict(cv::Mat image) {
    int _index = 0;
    double _doub = 0.0;

    cv::resize(image, image, cv::Size(32, 32));

    if (cv::imwrite(OUT_PATH, image) == false) {
        throw cv::Exception();
    }

    if (_predict(tdt, &_index, &_doub) < 0) {
        throw s_except("Predict function returned error value");
    } 

    return prediction_t(std::string(this->catagories[_index]), _doub);
}

