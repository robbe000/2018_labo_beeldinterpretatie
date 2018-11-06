#include <iostream>
#include <opencv2/opencv.hpp>
#include "skinpixels.h"

using namespace std;
using namespace cv;

int main(int argc, const char** argv) {
    Mat image1;


    //Helpboodschap configureren
    CommandLineParser parser(argc, argv,
    "{help | |Met dit programma dienen twee parameters meegegeven te worden}"
    "{img1 | |Het absolute pat van de eerste afbeelding -> afbeelding waarvan de skinpixels gesegmenteerd zullen worden)}");

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

    //Afbeelding 1 inlezen + nakijken of dit gelukt is
    cout << "File " << img1 << " inlezen..." << endl;
    image1 = imread(img1, CV_LOAD_IMAGE_COLOR);
    if(!image1.data)
    {
        cout <<  "Kon img1 niet openen" << endl ;
        return -1;
    }

    //Afbeelding 1 (grijs) weergeven
    namedWindow( "Afbeelding 1 origineel", WINDOW_AUTOSIZE);
    imshow( "Afbeelding 1 origineel", image1 );
    waitKey(0);

    skinpixels(image1);


    return 0;
}

