#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/ml.hpp"
#include <string.h>

#include "achtergrond.h"

using namespace std;
using namespace cv;
using namespace ml;

Mat tekenAchtergrond(void) {
    Mat achtergrond;

    achtergrond.create(1024, 750, CV_32FC1);

    //Heel de achtergrond op groen zetten
    for(int c=0; c<achtergrond.cols; c++) {
        for(int r=0; r<achtergrond.rows; r++) {
            achtergrond.at<Vec3b>(r, c)[0] = 0;
            achtergrond.at<Vec3b>(r, c)[1] = 255;
            achtergrond.at<Vec3b>(r, c)[2] = 0;
        }
    }

    return achtergrond;
}
