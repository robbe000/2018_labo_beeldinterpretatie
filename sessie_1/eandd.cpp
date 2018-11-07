#include "eandd.h"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void eandd(Mat image) {
    Mat output1;        //OTSU Thresholding
    Mat output2;        //Histogram equalization
    Mat output3;        //HE + Thresholding
    Mat output4;
    Mat output5;
    Mat output6;
    Mat output7;

    cout << "text segmenteren..." << endl;

    //OTSU Thresholding
    output1 = Mat::zeros(image.rows, image.cols, CV_8UC1);
    threshold(image, output1, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

    cout << "OTSU Thresholding..."  << endl;
    namedWindow( "Afbeelding 1 binaire textpixels", WINDOW_AUTOSIZE);
    imshow( "Afbeelding 1 binaire textpixels", output1 );
    waitKey(0);
    //We ontdekken hierbij een probleem -> de belichting is niet gelijk



    //Dit probleem proberen op te lossen met histogram equalization
    cout << "Histogram Equalization..." << endl;
    equalizeHist(image, output2);
    //Afbeelding met HE weergeven maar zonder thresholding
    namedWindow("HE", WINDOW_AUTOSIZE);
    imshow("HE", output2);
    //Afbeelding met HE weergeven en met thresholding
    output3 = Mat::zeros(image.rows, image.cols, CV_8UC1);
    threshold(output2, output3, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    namedWindow("HET", WINDOW_AUTOSIZE);
    imshow("HET", output3);
    waitKey(0);
    //Dit blijkt een beetje beter te zijn maar niet heel veel

    //Opening
    erode(output3, output4, Mat(), Point(-1, -1), 2);
    namedWindow("Opening", WINDOW_AUTOSIZE);
    imshow("Opening", output4);
    dilate(output3, output5, Mat(), Point(-1, -1), 2);
    namedWindow("Dilatie", WINDOW_AUTOSIZE);
    imshow("Dilatie", output5);

    //Contouren vinden
    vector<vector<Point>> cont;
    findContours(output4, cont, CV_RETR_EXTERNAL, CHAIN_APPROX_NONE);

    RNG rng(12345);
    Mat drawing = Mat::zeros( image.size(), CV_8UC3 );
    vector<Vec4i> hierarchy;
    for( int i = 0; i< cont.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        drawContours( drawing, cont, i, color, 2, 8, hierarchy, 0, Point() );
    }

    namedWindow("tekening", WINDOW_AUTOSIZE);
    imshow("tekening", drawing);

    waitKey(0);
    return;
}


