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

Mat tekenAchtergrond(int status) {
    Mat achtergrond;
    achtergrond = Mat::zeros(600, 1000, CV_8UC3 );

    //Heel de achtergrond op groen zetten
    for(int c=0; c<achtergrond.cols; c++) {
        for(int r=0; r<achtergrond.rows; r++) {
            achtergrond.at<Vec3b>(r,c)[0] = 0;
            achtergrond.at<Vec3b>(r,c)[1] = 200;
            achtergrond.at<Vec3b>(r,c)[2] = 100;

        }
    }

    if(status == STAT_WAIT_HANDDET) {
        tekenBoodschap(achtergrond, "HAND DETECTEREN...");
    }

    if(status == STAT_HAND_TRACK) {
        tekenBoodschap(achtergrond, "HAND TRACKEN...");
    }

    return achtergrond;
}

Mat tekenTracker(int status, Rect2d positie) {
    Mat achtergrond = tekenAchtergrond(status);

    rectangle(achtergrond, positie, Scalar(255, 0, 0), 2, 1);

    return achtergrond;
}

void tekenBoodschap(Mat achtergrond, String text) {
    putText(achtergrond, text, Point(40, 40), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 255), 2, LINE_AA);
}
