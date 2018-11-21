#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

vector<Point> template_match_multiple(Mat input_img, Mat template_img, int method, int weergeven);
Mat equalizeIntensity(const Mat& inputImage);

