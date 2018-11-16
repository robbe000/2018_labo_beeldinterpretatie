#include <iostream>
#include <opencv2/opencv.hpp>
#include <string.h>
#include "template_match.h"

using namespace std;
using namespace cv;

Point template_match(Mat input_img, Mat template_img, int method) {
    Mat result;
    double minVal, maxVal;
    Point minLoc, maxLoc, matchLoc;

    //De resultaat afbeelding aanmaken
    int result_cols = input_img.cols - template_img.cols + 1;
    int result_rows = input_img.rows - template_img.rows + 1;
    result.create(result_rows, result_cols, CV_32FC1);

    //Template matching uitvoeren
    matchTemplate(input_img, template_img, result, method);

    //Het resultaat normaliseren
    normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

    //De resulaatmatrix weergeven
    /*
    namedWindow("Resultaat Matrix");
    imshow("Resultaat Matrix", result);
    waitKey(0);
    */

    //Het minimum en maximum bepalen v/h resultaat
    minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

    //Afhankelijk van de gekozen methode het minimum of maximum gebruiken
    if( method  == CV_TM_SQDIFF || method == CV_TM_SQDIFF_NORMED ) {
        matchLoc = minLoc;
    } else {
        matchLoc = maxLoc;
    }

    return matchLoc;
}
