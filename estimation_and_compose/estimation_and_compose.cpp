#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/stitching.hpp"

#include <iostream>

#define DEBUG_PRINT

#ifdef DEBUG_PRINT
#define DEBUG_PRINT_ERR(x) (std::cerr << x << std::endl)
#define DEBUG_PRINT_OUT(x) (std::cout << x << std::endl)
#else
#define DEBUG_PRINT_ERR(x)
#define DEBUG_PRINT_OUT(x)
#endif

using namespace std;
using namespace cv;

bool try_gpu = false;
float imshow_scale_factor = 0.2f;

int main()
{
    Mat im1 = imread("../sample_image/test_image1.jpg");
    Mat im2 = imread("../sample_image/test_image2.jpg");
    Mat im3 = imread("../sample_image/test_image3.jpg");

    vector<Mat> imgs;
    imgs.push_back(im1);
    imgs.push_back(im2);
    imgs.push_back(im3);

    Mat output;
    Ptr<Stitcher> stitcher = Stitcher::create(Stitcher::PANORAMA, try_gpu);

    Stitcher::Status status = stitcher->estimateTransform(imgs);

    if (status != Stitcher::OK)
    {
        DEBUG_PRINT_ERR("Fail stitching: ");
        switch(status)
        {
            case Stitcher::ERR_NEED_MORE_IMGS :
                DEBUG_PRINT_ERR("Need more images");
                break;
            case Stitcher::ERR_HOMOGRAPHY_EST_FAIL :
                DEBUG_PRINT_ERR("Homography estimation failed");
                break;
            case Stitcher::ERR_CAMERA_PARAMS_ADJUST_FAIL :
                DEBUG_PRINT_ERR("Camera parameter adjustment failed");
                break;
            default :
                DEBUG_PRINT_ERR("Unknown Error");
        }
        return -1;
    }

    stitcher->composePanorama(output);
    
    DEBUG_PRINT_OUT("resize output panorama image and show it");
    Mat tmp;
    resize(output, tmp, Size(), imshow_scale_factor, imshow_scale_factor);
    imshow("Panorama image", tmp);
    waitKey(0);

    return 0;
}