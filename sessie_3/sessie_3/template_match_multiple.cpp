#include <iostream>
#include <opencv2/opencv.hpp>
#include <string.h>
#include "template_match.h"

using namespace std;
using namespace cv;

Point template_match_multiple(Mat input_img, Mat template_img, int method) {
    Mat result, mul_result, input_img_hist;
    double minVal, maxVal, hold;
    Point minLoc, maxLoc, matchLoc;

    //De resultaat afbeelding aanmaken
    int result_cols = input_img.cols - template_img.cols + 1;
    int result_rows = input_img.rows - template_img.rows + 1;
    result.create(result_rows, result_cols, CV_32FC1);
    mul_result.create(result_rows, result_cols, CV_32FC1);

    //Inputafbeelding histequal
    equalizeHist(input_img, input_img_hist);

    //Template matching uitvoeren
    matchTemplate(input_img, template_img, result, method);

    //Het resultaat normaliseren
    normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
    result = 1-result;

    //De resulaatmatrix weergeven
    namedWindow("Resultaat Matrix");
    imshow("Resultaat Matrix", result);
    waitKey(0);


    //Itereren over de afbeelding en minima of maxima aanduiden
    threshold(result, mul_result, 0.9, 1, THRESH_BINARY);

    namedWindow("hold");
    imshow("hold", mul_result);
    waitKey(0);

    return matchLoc;
}
