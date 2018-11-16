#include <iostream>
#include <opencv2/opencv.hpp>
#include <string.h>
#include "template_match.h"
#include "template_match_multiple.h"

using namespace std;
using namespace cv;

int main(int argc, const char** argv) {
    Mat input_img, template_img;
    Mat single_output_image;
    Point matchLoc;

    //Helpboodschap configureren
    CommandLineParser parser(argc, argv,
    "{help | |Met dit programma dienen twee parameters meegegeven te worden}"
    "{input | |Het absolute pat van de eerste afbeelding (Input image)}"
    "{template | |Het absolute pat van de twede afbeelding (Template)}");

    //Indien gevraagd helpboodschap afdrukken
    if(parser.has("help")) {
        parser.printMessage();
        return -1;
    }

    //Parameters inlezen
    string input(parser.get<string>("input"));
    string template1(parser.get<string>("template"));

    //Juiste hoeveelheid gegevens ingegeven?
    if((input == "")|(template1 == "")) {
        cerr << "Parameters niet correct opgegeven!"  << endl;
        parser.printMessage();
        return -1;
    }

    //Afbeeldingen inlezen + nakijken of dit gelukt is
    cout << "File " << input << " inlezen..." << endl;
    input_img = imread(input, CV_LOAD_IMAGE_COLOR);
    if(!input_img.data)
    {
        cout <<  "Kon input niet openen" << endl ;
        return -1;
    }

    //Afbeeldingen inlezen + nakijken of dit gelukt is
    cout << "File " << template1 << " inlezen..." << endl;
    template_img = imread(template1, CV_LOAD_IMAGE_COLOR);
    if(!template_img.data)
    {
        cout <<  "Kon template niet openen" << endl ;
        return -1;
    }

    //Originele afbeeldingen weergeven
    /*namedWindow("Input image");
    imshow("Input image", input_img);

    namedWindow("Template");
    imshow("Template", template_img);

    waitKey(0);*/

    //1 match zoeken
    matchLoc = template_match(input_img, template_img, 0);

    //Rechthoekje tekenen rond gevonden gebied
    /*input_img.copyTo(single_output_image);
    rectangle( single_output_image, matchLoc, Point( matchLoc.x + template_img.cols , matchLoc.y + template_img.rows ), Scalar::all(0), 2, 8, 0 );
    namedWindow("Output");
    imshow("Output", single_output_image);
    waitKey(0);*/

    //Meerdere matches zoeken
    template_match_multiple(input_img, template_img, 0);

}
