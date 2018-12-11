////////////////////////////////////////////////////////////////////////////
//
//   Project     : Beamer Fun
//   Sub project : Test Detect Screen
//   Description :
//      Het scherm detecteren om zo te weten hoe zwaar de beweging
//      dient vergroot te worden.
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

using namespace std;
using namespace cv;

int main(int argc, const char** argv) {
    Mat input_img;

    //Helpboodschap configureren
    CommandLineParser parser(argc, argv,
    "{help | |Met dit programma dienen twee parameters meegegeven te worden}"
    "{img1 | |Het absolute pat van de eerste afbeelding}");

    //Indien gevraagd helpboodschap afdrukken
    if(parser.has("help")) {
        parser.printMessage();
        return -1;
    }

    //Parameters inlezen
    string img1(parser.get<string>("img1"));

    //Juiste hoeveelheid gegevens ingegeven?
    if((img1 == "")) {
        cerr << "Parameters niet correct opgegeven!"  << endl;
        parser.printMessage();
        return -1;
    }

    //Afbeeldingen inlezen + nakijken of dit gelukt is
    cout << "File " << img1 << " inlezen..." << endl;
    input_img = imread(img1, CV_LOAD_IMAGE_COLOR);
    if(!input_img.data)
    {
        cout <<  "Kon img1 niet openen" << endl ;
        return -1;
    }

    //Enkel groene laag weergeven
    Mat split_img[3];
    Mat output;
    split(input_img, split_img);

    namedWindow("inputG");
    imshow("inputG", split_img[1]);
    waitKey(0);

    //Threshold toepassen
    threshold(split_img[1], output, 120, 255, CV_THRESH_BINARY);

    namedWindow("output");
    imshow("output", output);
    waitKey(0);

    //Grootste blob zoeken
    vector<vector<Point>> contours;
    output.convertTo(output, CV_8UC1);
    findContours(output, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    //Een blob gevonden?
    if(contours.size() > 0) {
        vector<Point> max_contour = contours[0];

        //Grootste blob zoeken -> Dit is waarschijnlijk het groene scherm
        for(uint i = 0; i<contours.size(); i++) {
            if(contourArea(max_contour) < contourArea(contours[i])) {
                max_contour = contours[i];
            }
        }

        //drawContours(input_img, max_contours, -1, 255, 3);
        rectangle(input_img, boundingRect(max_contour), Scalar(255, 0, 0), 3);
        namedWindow("output1");
        imshow("output1", input_img);
        waitKey(0);

        float schaal = (float)input_img.cols/boundingRect(max_contour).width;
        cout << schaal << endl;
    }

}
