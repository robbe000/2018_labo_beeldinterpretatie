#include <iostream>
#include <opencv2/opencv.hpp>
#include "skinpixels.h"
#include "text.h"
#include "eandd.h"

using namespace std;
using namespace cv;

int main(int argc, const char** argv) {
    Mat image1, image2, image3; //Ingelezen afbeeldingen (origineel)


    //Helpboodschap configureren
    CommandLineParser parser(argc, argv,
    "{help | |Met dit programma dienen twee parameters meegegeven te worden}"
    "{img1 | |Het absolute pat van de eerste afbeelding -> afbeelding waarvan de skinpixels gesegmenteerd zullen worden)}"
    "{img2 | |Het absolute pat van de twede afbeelding -> afbeelding waarvan de tekst gesegmenteerd zal worden.}"
    "{img3 | |Het absolute pat van de derde afbeelding -> Erosie en dilatie}");

    //Indien gevraagd helpboodschap afdrukken
    if(parser.has("help")) {
        parser.printMessage();
        return -1;
    }

    //Parameters inlezen
    string img1(parser.get<string>("img1"));
    string img2(parser.get<string>("img2"));
    string img3(parser.get<string>("img3"));

    //Juiste hoeveelheid gegevens ingegeven?
    if((img1 == "")|(img2 == "")|(img3=="")) {
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

    //Afbeelding 2 inlezen + nakijken of dit gelukt is
    cout << "File " << img2 << " inlezen..." << endl;
    image2 = imread(img2, CV_LOAD_IMAGE_GRAYSCALE);
    if(!image2.data)
    {
        cout <<  "Kon img2 niet openen" << endl ;
        return -1;
    }

    //Afbeelding 3 inlezen + nakijken of dit gelukt is
    cout << "File " << img3 << " inlezen..." << endl;
    image3 = imread(img3, CV_LOAD_IMAGE_GRAYSCALE);
    if(!image3.data)
    {
        cout <<  "Kon img3 niet openen" << endl ;
        return -1;
    }

    //DE INGELEZEN AFBEELDINGE WEERGEVEN
    //Afbeelding 1 (grijs) weergeven
    //namedWindow( "Afbeelding 1 origineel", WINDOW_AUTOSIZE);
    //imshow( "Afbeelding 1 origineel", image1 );

    //namedWindow("Afeelding 2 origineel", WINDOW_AUTOSIZE);
    //imshow("Afeelding 2 origineel", image2);

    //namedWindow("Afeelding 3 origineel", WINDOW_AUTOSIZE);
    //imshow("Afeelding 3 origineel", image3);

    //waitKey(0);


    //De verschillende onderdelen van de opdracht zijn opgesplitst in functies die te vinden zijn in gelijknamige .h en .cpp files
    //skinpixels(image1);
    //text(image2);
    eandd(image3);


    return 0;
}

