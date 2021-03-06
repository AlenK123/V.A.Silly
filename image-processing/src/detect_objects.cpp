#include "detect_objects.hpp"

classifications_t detect_objects(cv::Mat &frame, ssptr &ss, model &keras_model, const int n_threads) {
    std::vector<bounding_box> boxes; 
    cv::Mat input_im(frame);

    /* resizing the frame to a processable size */
    cv::resize(frame, input_im, cv::Size((input_im.cols * HIGHT / input_im.rows), HIGHT));

    /* 
     * it's always better to set the base frame with the real frame data rather than a reference or
     * a copy of it, it solves a lot of nasty errors that just make the algorithm not work to get all
     * of the potential regions.
     */
    ss->setBaseImage(input_im);

    rois R = find_regions_of_interest(input_im, ss, n_threads);

    for (auto &roi : R) {
        try {
            prediction_t prediction = keras_model.predict(input_im(roi));
            if (prediction.second >= (double)0.4) {
                boxes.push_back(bounding_box(roi, prediction.first));
            }
        }
        catch (cv::Exception &e) {
            /* log OpenCV exceptions to log */
            std::fprintf(stderr, "[CPP] :: %s\n", e.what());
            std::fprintf(stdout, "[CPP] :: %s\n", e.what());
            break;
        }
        catch (s_except &e) {
            /* log program errors to log */
            std::fprintf(stderr, "[CPP] :: %s\n", e.what());
            std::fprintf(stdout, "[CPP] :: %s\n", e.what());
            break;
        }
        catch (std::exception &e) {
            /* log standard exceptions to log */
            std::fprintf(stderr, "[CPP] :: %s\n", e.what());
            std::fprintf(stdout, "[CPP] :: %s\n", e.what());
            break;
        }
    }

    return boxes;
}