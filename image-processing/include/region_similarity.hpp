#pragma once

#include <iostream> //remove when done

#include "opencv2/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui.hpp"

#define BIN_SIZE 25

/* 
 * calculates the color similarity between to regions 
 * **(input: image, neighboring regions)**
 * **(output: color similarity)**
*/
double color_similarity(cv::Mat image, cv::Rect _ri, cv::Rect _rj);
