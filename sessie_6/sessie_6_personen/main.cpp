#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/ml.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include <string.h>

#define HAARFACE "haarcascade_frontalface_alt.xml"
#define HAARFACEIN "lbpcascade_frontalface_improved.xml"

using namespace std;
using namespace cv;
using namespace ml;

void detect(Mat frame, HOGDescriptor descriptor) {
    vector<Rect> mensen;
    vector<double> score;
    Mat frame_grijs;

    //F&J werkt op grijwaarden afbeeldinge -> converten
    cvtColor(frame, frame_grijs, CV_BGR2GRAY);
    equalizeHist(frame_grijs, frame_grijs);

    descriptor.detectMultiScale(frame_grijs, mensen, score);

    for(uint i=0; i<mensen.size(); i++) {
        //Center bepalen
        Point center( mensen[i].x + mensen[i].width*0.5, mensen[i].y + mensen[i].height*0.5 );

        //Rond het center een ellipse tekenen
        ellipse( frame, center, Size( mensen[i].width*0.5, mensen[i].height*0.5), 0, 0, 360, Scalar( 0, 255, 0 ), 4, 8, 0 );

        //Waarschijnlijkheid weergeven
        putText(frame, to_string(score[i]), Point2f(mensen[i].x, mensen[i].y), CV_FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 255, 0), 2, LINE_AA);
    }
}

int main(int argc, const char** argv) {

    //Helpboodschap configureren
    CommandLineParser parser(argc, argv,
    "{help | |Met dit programma dienen twee parameters meegegeven te worden}"
    "{input | |Het absolute pat van het filmpje dat verwerkt moet worden. (mp4)}");

    //Indien gevraagd helpboodschap afdrukken
    if(parser.has("help")) {
        parser.printMessage();
        return -1;
    }

    //-------------------------------//
    //De video inlezen

    //Parameters inlezen
    string input(parser.get<string>("input"));

    //Juiste hoeveelheid gegevens ingegeven?
    if(input == "") {
        cerr << "Parameters niet correct opgegeven!"  << endl;
        parser.printMessage();
        return -1;
    }

    //Videostream openen
    VideoCapture video(input);

    //Kon de videostream geopend worden
    if(!video.isOpened()) {
        cerr << "Kon video " << input << " niet openen!" << endl;
        return -1;
    }

    cout << "Inlezen video gelukt!" << endl;

    //-------------------------------//
    //Model inlezen
    HOGDescriptor descriptor;
    descriptor.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

    //-------------------------------//
    //Frames inlezen + weergeven
    Mat frame;
    namedWindow("Video");

    while(video.read(frame)) {
        detect(frame, descriptor);
        imshow("Video", frame);
        waitKey(32);
    }

}
