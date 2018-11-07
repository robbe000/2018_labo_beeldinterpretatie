#include "text.h"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void text(Mat image) {
    Mat output1;        //OTSU Thresholding
    Mat output2;        //Histogram equalization
    Mat output3;        //HE + Thresholding
    Mat output4;        //CLAHE
    Mat output5;        //CLAHE + Thresholding
    Mat output6;        //Combo
    Mat output7;        //Combo / Thresholding

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



    //Een twede methode: CLAHE
    Ptr<CLAHE> clahe = createCLAHE();
    clahe->apply(image, output4);
    //Afbeelding met CLAHE weergeven maar zonder thresholding
    namedWindow("CLAHE", WINDOW_AUTOSIZE);
    imshow("CLAHE", output4);
    output5 = Mat::zeros(image.rows, image.cols, CV_8UC1);
    threshold(output4, output5, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    namedWindow("CLAHET", WINDOW_AUTOSIZE);
    imshow("CLAHET", output5);
    waitKey(0);
    //Te veel ruis




    //Misschien eens een combinatie van bijde proberen??
    clahe->apply(output2, output6);
    //Afbeelding met CLAHE weergeven maar zonder thresholding
    namedWindow("combo", WINDOW_AUTOSIZE);
    imshow("combo", output6);
    output7 = Mat::zeros(image.rows, image.cols, CV_8UC1);
    threshold(output6, output7, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    namedWindow("comboT", WINDOW_AUTOSIZE);
    imshow("comboT", output7);
    waitKey(0);
    //Werkt ook niet 100% naar behoren

    return;
}

