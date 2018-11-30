#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <string.h>

using namespace std;
using namespace cv;

Mat input_glob;
Mat input_glob_hsv;
vector<Vec3b> voorgrond, achtergrond;
int status;     //0 voorgrond detectren, 1 achtergrond detecteren

void mCallback(int event, int x, int y, int flags, void* userdata) {
    Vec3b hsv = input_glob_hsv.at<Vec3b>(x,y);

    if(event == EVENT_LBUTTONDOWN) {
        cout << "Links geklikt \t Pos: "<< x << " " << y << "\n";
        cout << "Pixel waarde: H:" << (int)hsv.val[0] << " S:" << (int)hsv.val[1] << " V:" << (int)hsv.val[2] << "\t";

        if(status == 0) {
            voorgrond.push_back(hsv);
            cout << "Aantal punten voorgrond: " << voorgrond.size();
        } else if (status == 1) {
            achtergrond.push_back(hsv);
            cout << "Aantal punten achtergrond: " << achtergrond.size();
        }

        cout << endl;
    } else if ( event == EVENT_RBUTTONDOWN ) {
        cout << "Laatste element verwijderen... \n";
        if(voorgrond.size() > 0) {
            if(status == 0) {
                voorgrond.pop_back();
                cout << "Aantal punten voorgrond: " << voorgrond.size() << endl;
            } else if (status == 1) {
                achtergrond.pop_back();
                cout << "Aantal punten voorgrond: " << achtergrond.size() << endl;
            }
        }
    }
}

void berekenen(void) {
    imshow("Beeld", input_glob);
}

int main(int argc, const char** argv) {
    Mat input_img;

    //Helpboodschap configureren
    CommandLineParser parser(argc, argv,
    "{help | |Met dit programma dienen twee parameters meegegeven te worden}"
    "{input | |Het absolute pat van de eerste afbeelding (Input image)}");

    //Indien gevraagd helpboodschap afdrukken
    if(parser.has("help")) {
        parser.printMessage();
        return -1;
    }

    //Parameters inlezen
    string input(parser.get<string>("input"));

    //Juiste hoeveelheid gegevens ingegeven?
    if(input == "") {
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

    GaussianBlur(input_img, input_img, Size(5, 5), 0);

    //De input afbeedling globaal beschikbaar maken
    input_img.copyTo(input_glob);

    //HSV eenmalig opsplitsen voor het verzamelen van de data na het klikken
    cvtColor(input_img, input_glob_hsv, COLOR_BGR2HSV);

    namedWindow("Beeld");

    //Callback functie muis voor selecteren van de pixels
    setMouseCallback("Beeld", mCallback, NULL);

    cout << "Goede voorbeelden inlezen\n----------------------------------------" << endl;
    //Voorgrond detecteren
    status = 0;
    berekenen();




    //Wachten op switchen van modus
    waitKey(0);
    cout << "Slechte voorbeelden inlezen\n----------------------------------------" << endl;
    //Achtergrond detecteren
    status = 1;
    berekenen();





    waitKey(0);
    //Gegevens analyseren
    cout << "Gegevens verwerken\n---------------------------------------------------"  << endl;

    //trainingdata en label aanmaken
    //iets met voncat

}
