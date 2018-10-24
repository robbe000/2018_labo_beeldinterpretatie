#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, const char** argv) {
    Mat image1;
    Mat image2;
    Mat splitImg[3];
    Mat grijs;
    Mat tekenen;

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

    if(false) {
    //Afbeelding 1 (grijs) weergeven
    namedWindow( "Afbeelding 1 grijs", WINDOW_AUTOSIZE);
    imshow( "Afbeelding 1 grijs", image1 );

    //Afbeelding 2 (grijs) weergeven
    namedWindow( "Afbeelding 2 kleur", WINDOW_AUTOSIZE);
    imshow( "Afbeelding 2 kleur", image2 );

    //Wachten tot men op een toets duuwt
    waitKey(0);





    //Kleurenafbeelding opsplitsen in bgr
    cout << "Kleurenafbeelding opsplitsen in bgr.." << endl;
    split(image2, splitImg);

    //Blauw weergeven
    namedWindow( "Afbeelding B", WINDOW_AUTOSIZE);
    imshow("Afbeelding B", splitImg[0]);

    //Groen weergeven
    namedWindow( "Afbeelding G", WINDOW_AUTOSIZE);
    imshow("Afbeelding G", splitImg[1]);

    //Rood weergeven
    namedWindow( "Afbeelding R", WINDOW_AUTOSIZE);
    imshow("Afbeelding R", splitImg[2]);

    //Wachten tot men op een toets duuwt
    waitKey(0);



    //Kleur omvormen naar grijs
    cout << "Kleurenafbeelding omvormen naar grijswaarden..."  << endl;
    cvtColor(image2, grijs, COLOR_RGB2GRAY);

    //Omgevormde afbeelding weergeven
    namedWindow("Grijswaarden", WINDOW_AUTOSIZE);
    imshow("Grijswaarden", grijs);

    //Wachten tot men op een toets duuwt
    waitKey(0);




    //Waardes naar CMD printen
    cout << "Waarden van grijswaardenafbeelding afdrukken"  << endl;
    for(int i=0; i<grijs.rows ; i++) {
        for(int j=0; j<grijs.cols ; j++) {
            cout << (int)grijs.at<uchar>(i, j) << endl;
        }
    }
    }

    //Twee figuren tekenen op een canvas
    int dimentie = 255;
    tekenen = Mat::zeros(dimentie, dimentie, CV_8UC3 );

    //Elips tekenen
    ellipse( tekenen,
           Point( dimentie/3.0, dimentie/2.0 ),
           Size( dimentie/4.0, dimentie/16.0 ),
           20,                           //Hoek
           0,
           360,
           Scalar( 255, 0, 0 ),
           2,                               //Dikte
           8 );                             //Lijtype

     circle( tekenen,
             Point( (dimentie/3.0)*2, dimentie/2.0 ),
             dimentie/32.0,
             Scalar( 0, 0, 255 ),
             -1,
             8 );

    //Afbeelding weergeven
    namedWindow("Tekening", WINDOW_AUTOSIZE);
    imshow("Tekening", tekenen);

    waitKey(0);

    return 0;
}


