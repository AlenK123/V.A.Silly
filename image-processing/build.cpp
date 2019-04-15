#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

int main(int argc, char ** argv) {
	if (argc < 2) {
		std::cerr << argv[0] << " <img path>" << std::endl;
		return EXIT_FAILURE;
	}

	cv::Mat src;
	cv::Mat dst;
	src = cv::imread(argv[1]);
	
	cv::resize(src, dst, cv::Size(32, 32));
	cv::resize(dst, dst, cv::Size(500, 500));
	
	cv::imshow("Image", dst);
	cv::waitKey(0);
	
	return EXIT_SUCCESS;
}
