#include "skinpixels.h"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void skinpixels(Mat image) {
    Mat output1;        //Binaire afbeelding
    Mat output2;        //Kleuren weergeven
    Mat splitImg[3];    //De inputafbeelding gesplitst

    cout << "Skinpixels segmenteren..." << endl;

    //Nieuwe afbeeldigen aanmaken
    output1 = Mat::zeros(image.rows+1, image.cols+1, CV_8UC3 );
    output2 = Mat::zeros(image.rows+1, image.cols+1, CV_8UC3 );

    //De inputafbeelding splitsen in BGR
    split(image, splitImg);

    //Over de afbeelding itereren en bekijken of de pixel tot de skin range behoord
    for(int i=0; i<image.rows ; i++) {
        for(int j=0; j<image.cols ; j++) {
            int BLUE = (int)splitImg[0].at<uchar>(i, j);
            int GREEN = (int)splitImg[1].at<uchar>(i, j);
            int RED = (int)splitImg[2].at<uchar>(i, j);
            //cout << "B: " << BLUE << " G: " << GREEN << " R: " << RED << endl;

            if((RED>95) && (GREEN>40) && (BLUE>20) && ((max(RED,max(GREEN,BLUE)) - min(RED,min(GREEN,BLUE)))>15) && (abs(RED-GREEN)>15) && (RED>GREEN) && (RED>BLUE)){
                output1.at<Vec3b>(i,j)[0] = 255;
                output1.at<Vec3b>(i,j)[1] = 255;
                output1.at<Vec3b>(i,j)[2] = 255;

                output2.at<Vec3b>(i,j) = image.at<Vec3b>(i,j);
            }
        }
    }

    cout << "Outputs weergeven..."  << endl;
    //Output afbeelding 1 weergeven
    namedWindow( "Afbeelding 1 binaire skinpixels", WINDOW_AUTOSIZE);
    imshow( "Afbeelding 1 binaire skinpixels", output1 );
    //Output afbeelding 2 weergeven (kleur)
    namedWindow( "Afbeelding 1 skinpixels", WINDOW_AUTOSIZE);
    imshow( "Afbeelding 1 skinpixels", output2 );
    waitKey(0);

    return;
}
