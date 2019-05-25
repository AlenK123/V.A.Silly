#pragma once
#include "opencv2/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui.hpp"

/* 
 * calculates the color similarity between to regions 
 * **(input: image, neighboring regions)**
 * **(output: color similarity)**
*/
double color_similarity(cv::Mat image, cv::Rect _ri, cv::Rect _rj);

/* 
 * calculates the texture similarity between to regions 
 * **(input: image, neighboring regions)**
 * **(output: texture similarity)**
*/
double texture_similarity(cv::Mat image, cv::Rect _ri, cv::Rect _rj);

/* 
 * calculates the size similarity between to regions 
 * **(input: image, neighboring regions)**
 * **(output: size similarity)**
*/
double size_similarity(cv::Mat image, cv::Rect _ri, cv::Rect _rj);
