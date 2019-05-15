#pragma once
#include <vector>
#include <set>

#include "opencv2/ximgproc/segmentation.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"

#include "neighboring_regions.hpp"

#define NEIGHBOR_TH (double)70

using namespace cv::ximgproc::segmentation;
using ssptr = cv::Ptr<SelectiveSearchSegmentation>;
using rois = std::vector<cv::Rect>;
using similarity_set = std::set<neighboring_regions>;
using bounding_box = std::pair<cv::Rect, const std::string>;

/**
 * takes all of the region proposals and emplaces all of the neighboring regions into a set
 * **(input: image, region proposals and similarity set)**
 * **(output: none)**
*/
void prepare_neighboring_rois(const cv::Mat &image, rois v, similarity_set &ss);

/*
 * calculates the analitical distance between two rectangles
 * **(input: recatangle a and rectangle b)**
 * **(output: distance between []a and []b)** 
*/
double analytical_distance(cv::Rect r1, cv::Rect r2);

/*
 * indicates if two regions of interest are neighboring
 * **(input: region a and region b)**
 * **(are a and b neighboring true/false)**
*/
bool are_rois_neighboring(cv::Rect r_i, cv::Rect r_j);

/*
 * makes a bunch of region proposals and pushes them onto an array
 * **(input: target image, selective search segmentation pointer)**
 * **(output: an array/vector of region proposals)**
*/
rois find_regions_of_interest(cv::Mat &image, ssptr &ss);


/*
 * remove similarities regarding s(ri, r\*\), s(rj, r\*\)
 * **(input: similarity set, neighboring regions instance)**
 * **(output: void)**
*/
similarity_set remove_instances(similarity_set &ss, std::set<neighboring_regions>::iterator ins);

/*
 * draws region proposals upon an image
 * **(input: image, region proposals)**
 * **(image)**
*/
cv::Mat draw_rois(cv::Mat image, std::vector<bounding_box> v);
