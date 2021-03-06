#include <iostream>
#include <opencv2/opencv.hpp>
#include <string.h>
#include "template_match_multiple.h"

using namespace std;
using namespace cv;

vector<Point> template_match_multiple(Mat input_img, Mat template_img, int method, int weergeven) {
    Mat result, mul_result;
    Point minLoc, maxLoc, matchLoc;
    vector<Point> punten_resultaat;

    //De resultaat afbeeldingen aanmaken
    int result_cols = input_img.cols - template_img.cols + 1;
    int result_rows = input_img.rows - template_img.rows + 1;
    result.create(result_rows, result_cols, CV_32FC1);
    mul_result.create(result_rows, result_cols, CV_32FC1);

    //Template matching uitvoeren
    matchTemplate(input_img, template_img, result, method);

    //Het resultaat normaliseren
    //---Omdat er soms geen resultaten gevonden zullen worden kunnen we niet normaliseren.
    //---Daarom de threshold toepassen op de niet genormaliseerde waarde!
    //normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

    /*if( method  == CV_TM_SQDIFF || method == CV_TM_SQDIFF_NORMED ) {
        result = 1-result;
    }*/

    //Globale gegevens bekijken
    double minVal1, maxVal1;
    minMaxLoc(result, &minVal1, &maxVal1, nullptr, nullptr);
    cout << "Globale gegevens Minval: " << minVal1 << " Maxval: " << maxVal1 << endl;

    if(weergeven != 0) {
        //De resulaatmatrix weergeven
        namedWindow("Resultaat Matrix");
        imshow("Resultaat Matrix", result);
        moveWindow("Resultaat Matrix", 0, 0);
    }

    //Minima of maxima aanduiden
    threshold(result, mul_result, 0.7, 1, THRESH_BINARY);

    if(weergeven != 0) {
        namedWindow("hold");
        imshow("hold", mul_result);
        moveWindow("hold", 1100, 0);
    }

    //Blob zoeken
    vector<vector<Point>> contours;
    mul_result.convertTo(mul_result, CV_8UC1);
    findContours(mul_result, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    for(uint i=0;i<contours.size();i++) {
        Mat blobImg;
        Point minLoc, maxLoc;
        double minVal, maxVal;

        Rect blob = boundingRect(contours[i]);
        blobImg = result(blob);

        minMaxLoc(blobImg, &minVal, &maxVal, &minLoc, &maxLoc);
        punten_resultaat.push_back(Point(blob.x+maxLoc.x, blob.y+maxLoc.y));
        rectangle(input_img, Point(blob.x+maxLoc.x, blob.y+maxLoc.y), Point(blob.x+maxLoc.x+template_img.cols, blob.y+maxLoc.y+template_img.rows), Scalar(0,255,255), 3, 8, 0);
        cout << maxVal << endl;
    }

    cout << "-------" << endl;
    if(weergeven != 0) {
        namedWindow("Template");
        imshow("Template", template_img);
        namedWindow("Output");
        imshow("Output", input_img);
        moveWindow("Output", 0, 800);
        waitKey(weergeven);
    }

    return punten_resultaat;
}
