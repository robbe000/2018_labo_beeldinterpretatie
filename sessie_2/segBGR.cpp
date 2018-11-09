#include <iostream>
#include <opencv2/opencv.hpp>
#include <string.h>
#include "segBGR.h"

using namespace std;
using namespace cv;

int sliderR = 0;
int sliderRM;
Mat image[4];

void segBGR(Mat imageIn[]) {
    cout << "Afbeeldingen segmenteren BGR..." << endl;

    //Input globaal beschikbaar maken
    for(int i=0; i<4; i++) {
        image[i] = imageIn[i];
    }

    //Slider rood maximum aanmaken
    namedWindow("SliderRood");
    createTrackbar( "Slider Rood", "SliderRood", &sliderR, 255, sliderRood );
    sliderRood(sliderR , 0);

    //Slider rood minimum aanmaken
    createTrackbar( "Slider Rood Min", "SliderRood", &sliderRM, 255, sliderRoodMin );
    sliderRood(sliderRM , 0);

    //Window initialiseren
    for(int i=0;i<4;i++) {
        namedWindow("Rood " + std::to_string(i));
    }

    waitKey(0);
}

void voerUit() {
    for(int i=0;i<4;i++) {
        Mat splitImage[3];
        Mat output;

        split(image[i], splitImage);

        threshold(splitImage[2], output, sliderRM, sliderR, CV_THRESH_BINARY);

        imshow("Rood " + std::to_string(i), output);
    }
}

void sliderRood( int val, void* ) {
    //cout << std::to_string(val) << endl;
    voerUit();
}

void sliderRoodMin(int val, void*) {
    voerUit();
}
