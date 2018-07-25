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

    vector<detail::CameraParams> cameras = stitcher->cameras();

    for(int i = 0; i < imgs.size(); i++)
    {
        DEBUG_PRINT_OUT("----Image index " << i << " camera parameters----");
        DEBUG_PRINT_OUT("Camera intrinsic matrix");
        DEBUG_PRINT_OUT(cameras[i].K());

        DEBUG_PRINT_OUT("Focal length : " << cameras[i].focal);

        DEBUG_PRINT_OUT("Aspect ratio : " << cameras[i].aspect);

        DEBUG_PRINT_OUT("Principle Point X : " << cameras[i].ppx);

        DEBUG_PRINT_OUT("Principle Point Y : " << cameras[i].ppy);

        DEBUG_PRINT_OUT("Rotation matrix");
        DEBUG_PRINT_OUT(cameras[i].R);

        DEBUG_PRINT_OUT("Translation matrix");
        DEBUG_PRINT_OUT(cameras[i].t);
        DEBUG_PRINT_OUT(" ");
    }

    return 0;
}