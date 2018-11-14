#include <iostream>
#include <opencv2/opencv.hpp>
#include <string.h>
#include "segHSV.h"

using namespace std;
using namespace cv;

Mat hsvImage[4];
Mat input[4];
int hl, hh, sl, sh, vl, vh;

void segHSV(Mat imageIn[]) {

    //Omvormen naar HSV + globaal beschikbaar maken
    for(int i=0; i<4;i++) {
        cvtColor(imageIn[i], hsvImage[i], COLOR_BGR2HSV);
        input[i] = imageIn[i];
    }

    //Controle variablen initialiseren
    hl=0;
    hh=0;
    sl=0;
    sh=0;
    vl=0;
    vh=0;

    //Slider h maximum aanmaken
    namedWindow("Sliders");
    createTrackbar( "Hue low", "Sliders", &hl, 180, voeruit );
    createTrackbar( "Hue high", "Sliders", &hh, 180, voeruit );
    createTrackbar( "Sat low", "Sliders", &sl, 255, voeruit );
    createTrackbar( "Sat high", "Sliders", &sh, 255, voeruit );
    createTrackbar( "Value low", "Sliders", &vl, 250, voeruit );
    createTrackbar( "Value high", "Sliders", &vh, 250, voeruit );


    //Window initialiseren
    for(int i=0;i<4;i++) {
        namedWindow("HSV " + std::to_string(i));
    }

    waitKey(0);
}

void voeruit(int val, void*) {
    cout << val << endl;

    //Voor alle vier de afbeeldingen de operaties uitvoeren
    for(int i=0;i<4;i++) {
        Mat output, output1[4];
        vector<vector<Point>> contours;
        int maximum;

        for(int j=0;j<4;j++) {
            output1[j] = input[j];
        }

        //HSV waarden thresholden
        inRange(hsvImage[i], Scalar(hl, sl, vl), Scalar(hh, sh, vh), output);

        //Contouren zoeken
        findContours(output, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        //Grootste bepalen
        maximum = 0;
        for(uint k=1;k<contours.size();k++) {
            if(contourArea(contours[k]) > contourArea(contours[maximum])) {
                maximum = k;
            }
        }

        //De grootste blob tekenen op de afbeelding
        drawContours(output1[i], contours, maximum, Scalar(0,255,0), 3);

        //Resultaat weergeven
        imshow("HSV " + std::to_string(i), output1[i]);
    }
}
