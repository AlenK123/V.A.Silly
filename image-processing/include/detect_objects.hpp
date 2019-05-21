#pragma once
#include <vector>

#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "rois.hpp"
#include "model.hpp"
#include "s_except.hpp"

using classifications_t = std::vector<bounding_box>;

classifications_t detect_objects(cv::Mat &frame, ssptr &ss, model &keras_model);