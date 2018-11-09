#include <iostream>
#include <opencv2/opencv.hpp>
#include <string.h>
#include "segBGR.h"

using namespace std;
using namespace cv;

// TODO (robbe#1#): bounding boxen, connected components

int main(int argc, const char** argv) {
    Mat image[4];


    //Helpboodschap configureren
    CommandLineParser parser(argc, argv,
    "{help | |Met dit programma dienen twee parameters meegegeven te worden}"
    "{img1 | |Het absolute pat van de eerste afbeelding (verkeersbord)}"
    "{img2 | |Het absolute pat van de twede afbeelding (verkeersbord)}"
    "{img3 | |Het absolute pat van de derde afbeelding (verkeersbord)}"
    "{img4 | |Het absolute pat van de vierde afbeelding (verkeersbord)}");

    //Indien gevraagd helpboodschap afdrukken
    if(parser.has("help")) {
        parser.printMessage();
        return -1;
    }

    //Parameters inlezen
    string img1(parser.get<string>("img1"));
    string img2(parser.get<string>("img2"));
    string img3(parser.get<string>("img3"));
    string img4(parser.get<string>("img4"));

    //Juiste hoeveelheid gegevens ingegeven?
    if((img1 == "")|(img2 == "")|(img3=="")|(img4=="")) {
        cerr << "Parameters niet correct opgegeven!"  << endl;
        parser.printMessage();
        return -1;
    }

    //Afbeeldingen inlezen + nakijken of dit gelukt is
    cout << "File " << img1 << " inlezen..." << endl;
    image[0] = imread(img1, CV_LOAD_IMAGE_COLOR);
    if(!image[0].data)
    {
        cout <<  "Kon img1 niet openen" << endl ;
        return -1;
    }

    //Afbeelding 2 inlezen + nakijken of dit gelukt is
    cout << "File " << img2 << " inlezen..." << endl;
    image[1] = imread(img2, CV_LOAD_IMAGE_COLOR);
    if(!image[1].data)
    {
        cout <<  "Kon img2 niet openen" << endl ;
        return -1;
    }

    //Afbeelding 3 inlezen + nakijken of dit gelukt is
    cout << "File " << img3 << " inlezen..." << endl;
    image[2] = imread(img3, CV_LOAD_IMAGE_COLOR);
    if(!image[2].data)
    {
        cout <<  "Kon img3 niet openen" << endl ;
        return -1;
    }

    //Afbeelding 4 inlezen + nakijken of dit gelukt is
    cout << "File " << img4 << " inlezen..." << endl;
    image[3] = imread(img4, CV_LOAD_IMAGE_COLOR);
    if(!image[3].data)
    {
        cout <<  "Kon img4 niet openen" << endl ;
        return -1;
    }

    //Alle ingelezen afbeeldingen weergevenµ
    /*
    for(int i=0; i<4; i++) {
        namedWindow( "Afbeelding " + std::to_string(i) +" origineel", WINDOW_AUTOSIZE);
        imshow( "Afbeelding " + std::to_string(i) +" origineel", image[i] );
    }
    waitKey(0);
    */

    segBGR(image);
}
