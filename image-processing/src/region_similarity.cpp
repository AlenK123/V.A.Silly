#include "region_similarity.hpp"

double color_similarity(cv::Mat image, cv::Rect _ri, cv::Rect _rj) {
    cv::Mat regions[2] = { image(_ri), image(_rj) };
    std::vector<cv::Mat> bins;
    
    static const int hist_size = BIN_SIZE;
    static const float range[] = { 0, 256 } ;
    static const float* hist_range = { range };

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
        
        /* Normalize the result to [ 0, histImage.rows ] */
        /*               height, width */
        cv::Mat histImage(400, 512, CV_8UC3, cv::Scalar( 0,0,0));

        normalize(b_hist, b_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
        normalize(g_hist, g_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
        normalize(r_hist, r_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());

        /* concatnating the histogram to make a dimentiality of n = 75 (25 bins x  3 channels) */
        cv::vconcat(std::vector<cv::Mat>({r_hist, g_hist, r_hist}), dst);

        bins.push_back(dst);
    }
    
    /* calculating similarity: more info in the README file */
    for (int k = 0; k < (hist_size * 3); k++) {
        sum += std::min(bins[0].at<double>(k), bins[1].at<double>(k));
    }

    return sum;
}

double texture_similarity(cv::Mat image, cv::Rect _ri, cv::Rect _rj) {
    
    return 0.0;
}