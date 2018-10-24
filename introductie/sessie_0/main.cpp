#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, const char** argv) {
    Mat image1;
    Mat image2;


    //Helpboodschap configureren
    CommandLineParser parser(argc, argv,
    "{help | |Met dit programma dienen twee parameters meegegeven te worden}"
    "{img1 | |Het absolute pat van de eerste afbeelding (zwart wit)}"
    "{img2 | |Het absolute pat van de twede afbeelding (kleur)}");

    //Indien gevraagd helpboodschap afdrukken
    if(parser.has("help")) {
        parser.printMessage();
        return -1;
    }

    //Parameters inlezen
    string img1(parser.get<string>("img1"));
    string img2(parser.get<string>("img2"));

    //Juiste hoeveelheid gegevens ingegeven?
    if((img1 == "")|(img2 == "")) {
        cerr << "Parameters niet correct opgegeven!"  << endl;
        parser.printMessage();
        return -1;
    }

    //Afbeelding 1 inlezen + nakijken of dit gelukt is
    cout << "File " << img1 << " inlezen..." << endl;
    image1 = imread(img1, CV_LOAD_IMAGE_GRAYSCALE);
    if(!image1.data)
    {
        cout <<  "Kon img1 niet openen" << endl ;
        return -1;
    }

    //Afbeelding 2 inlezen + nakijken of dit gelukt is
    cout << "File " << img2 << " inlezen..." << endl;
    image2 = imread(img2, CV_LOAD_IMAGE_COLOR);
    if(!image2.data)
    {
        cout <<  "Kon img2 niet openen" << endl ;
        return -1;
    }

    //Afbeelding 1 (grijs) weergeven
    namedWindow( "Afbeelding 1 grijs", WINDOW_AUTOSIZE );
    imshow( "Afbeelding 1 grijs", image1 );

    //Afbeelding 2 (grijs) weergeven
    namedWindow( "Afbeelding 2 kleur", WINDOW_AUTOSIZE );
    imshow( "Afbeelding 2 kleur", image2 );

    //Wachten tot men op een toets duuwt
    waitKey(0);

    return 0;
}


