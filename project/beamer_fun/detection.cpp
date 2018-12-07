#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/ml.hpp"
#include <string.h>
#include "detection.h"

using namespace std;
using namespace cv;

Rect detect(Mat frame, CascadeClassifier haar_face_classifier) {
    vector<Rect> gezichten;
    vector<int> score;
    Mat frame_grijs;
    Rect gezicht;
    gezicht.x = 0;
    gezicht.y = 0;

    //F&J werkt op grijwaarden afbeeldinge -> converten
    cvtColor(frame, frame_grijs, CV_BGR2GRAY);
    equalizeHist(frame_grijs, frame_grijs);

    haar_face_classifier.detectMultiScale(frame_grijs, gezichten, score, 1.05, 3);

    for(uint i=0; i<gezichten.size(); i++) {
        if(score[i]>150) {
            //Center bepalen
            Point center( gezichten[i].x + gezichten[i].width*0.5, gezichten[i].y + gezichten[i].height*0.5 );

            //Rond het center een ellipse tekenen
            ellipse( frame, center, Size( gezichten[i].width*0.5, gezichten[i].height*0.5), 0, 0, 360, Scalar( 0, 255, 0 ), 4, 8, 0 );

            //Waarschijnlijkheid weergeven
            putText(frame, to_string(score[i]), Point2f(gezichten[i].x, gezichten[i].y), CV_FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 255, 0), 2, LINE_AA);

            //Wegschrijven voor verdere verwerking
            gezicht = gezichten[i];
        }
    }

    return gezicht;
}
