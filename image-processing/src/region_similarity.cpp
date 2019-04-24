#include "region_similarity.hpp"

double color_similarity(cv::Mat image, cv::Rect _ri, cv::Rect _rj) {
    cv::Mat regions[2] = { image(_ri), image(_rj) };
    std::vector<cv::Mat> bins;
    
    int hist_size = BIN_SIZE;
    float range[] = { 0, 256 } ;
    const float* hist_range = { range };

    double sum = 0;

    for (auto r : regions) {
        std::vector<cv::Mat> rgb_planes;
        cv::Mat b_hist, g_hist, r_hist;
        cv::Mat dst;

        /* deviding the image into the 3 channels RGB */
        cv::split(r, rgb_planes);
        
        /* Compute the histograms: */
        cv::calcHist(&rgb_planes[0], 1, 0, cv::Mat(), b_hist, 1, &hist_size, &hist_range, true, false);
        cv::calcHist(&rgb_planes[1], 1, 0, cv::Mat(), g_hist, 1, &hist_size, &hist_range, true, false);
        cv::calcHist(&rgb_planes[2], 1, 0, cv::Mat(), r_hist, 1, &hist_size, &hist_range, true, false);

        cv::vconcat(std::vector<cv::Mat>({r_hist, g_hist, r_hist}), dst);

        bins.push_back(dst);
    }
    
    /* calculating similarity: more info in the README file */
    for (int k = 0; k < (hist_size * 3); k++) {
        sum += std::min(bins[0].at<double>(k), bins[1].at<double>(k));
    }

    return sum;
}
