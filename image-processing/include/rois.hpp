#pragma once
#include <thread>
#include <vector>
#include <set>

#include <opencv2/ximgproc/segmentation.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "neighboring_regions.hpp"

#define NEIGHBOR_TH (double)70

#define HIGHT 256

using namespace cv::ximgproc::segmentation;
using ssptr = cv::Ptr<SelectiveSearchSegmentation>;
using rois = std::vector<cv::Rect>;
using similarity_set = std::set<neighboring_regions>;
using bounding_box = std::pair<cv::Rect, const std::string>;


/**
 * takes a portion form the region proposals and emplaces all of the 
 * neighboring regions int the into a set portion
 * **(input: image, region proposals range and similarity set)**
 * **(output: none)**
*/
void _prepare_neighboring_rois(const cv::Mat &image, rois::iterator b, rois::iterator e, similarity_set &ss);

/**
 * takes all of the region proposals and emplaces all of the neighboring regions into a set
 * with thread devision
 * **(input: image, region proposals, similarity set, number of threads)**
 * **(output: none)**
*/
void prepare_neighboring_rois(const cv::Mat &image, rois v, similarity_set &ss, const int n_threads);

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
 * **(input: target image, selective search segmentation pointern, number of processing threads)**
 * **(output: an array/vector of region proposals)**
*/
rois find_regions_of_interest(cv::Mat &image, ssptr &ss, const int n_threads);

/*
 * remove similarities regarding s(ri, r*), s(rj, r*)
 * **(input: similarity set, neighboring regions instance)**
 * **(output: void)**
*/
similarity_set remove_instances(similarity_set &ss, std::set<neighboring_regions>::iterator ins);

/*
 * rescales a ROI to a size proportional to the original image
 * **(input original image, unproportioned ROI)**
 * **(proportioned ROI)**
*/
cv::Rect rescale(cv::Mat &image, cv::Rect roi);

/*
 * draws region proposals upon an image
 * **(input: image, region proposals)**
 * **(image)**
*/
cv::Mat draw_rois(cv::Mat image, std::vector<bounding_box> v);
