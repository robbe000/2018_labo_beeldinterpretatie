
////////////////////////////////////////////////////////////////////////////
//
//   Project     : Beamer Fun
//   Description :
//
//
//   Created On: 06/12/2018
//   Created By: Robbe Goovaerts robbe_goovaerts@telenet.be
////////////////////////////////////////////////////////////////////////////

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

int main(int argc, const char** argv) {
    VideoCapture vcap;
    Mat frame;

    //Helpboodschap configureren
    CommandLineParser parser(argc, argv,
    "{help | |Voorzie een live video}");

    //Indien gevraagd helpboodschap afdrukken
    if(parser.has("help")) {
        parser.printMessage();
        return -1;
    }

    if(!vcap.open("http://192.168.1.111:8080/video")) {
        cout << "Kon videostream niet openen!" << endl;
        return -1;
    }

    namedWindow("Live");
    namedWindow("Achtergrond");
    imshow("Achtergrond", tekenAchtergrond());
    waitKey(0);

    while(1) {
        if(!vcap.read(frame)) {
            cout << "Kon frame niet inlezen" << endl;
            return -1;
        }

        imshow("Live", frame);
        waitKey(30);
    }
}
