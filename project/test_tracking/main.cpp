////////////////////////////////////////////////////////////////////////////
//
//   Project     : Beamer Fun
//   Sub project : Test Tracking
//   Description :
//      Het testen van bepaalde tracking algoritmes.
//      Informatie gevonden op  -> Learn OpenCV (https://www.learnopencv.com/object-tracking-using-opencv-cpp-python/)
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

#define NUMAL 8

using namespace std;
using namespace cv;
using namespace ml;

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

    while(1) {
        //Videostream openen
        VideoCapture video(input);

        //Kon de videostream geopend worden
        if(!video.isOpened()) {
            cerr << "Kon video " << input << " niet openen!" << endl;
            return -1;
        }

        cout << "Inlezen video gelukt!" << endl;


        //-------------------------------//
        //Het eerste frame inlezen
        // + de gebruiker vragen een bbox te tekenen rond het te tracken object

        //Eerste frame inlezen
        cout << "Eerste frame inlezen + weergeven..." << endl;
        Mat frame;
        bool ok = video.read(frame);

        //De gebruiker het te tracken object laten aanduiden
        cout << "Duid het te tracken object aan en druk op enter!" << endl;
        Rect2d bbox;
        bbox = selectROI(frame, true);




        //-------------------------------//
        //De video frame per frame inlezen en een nieuwe bbox berekenen


        //Gebruiker de keuze geven welke tracker hij wenst ge gebruiken
        Ptr<Tracker> tracker;

        cout << "Welke tracker wens je te gebruiken?" << endl;
        String trackers[NUMAL] = {"BOOSTING", "MIL", "KCF", "TLD","MEDIANFLOW", "GOTURN", "MOSSE", "CSRT"};

        for(int i=0; i<NUMAL; i++){
            cout << i << ": " << trackers[i] << endl;
        }
        cout << "Geef een getal op:" << endl;

        int alnummer;
        cin >> alnummer;

        //Enkele checs uitvoeren op input
        if(alnummer < 0) {
            cout << "Het getal is te klein!" << endl;
            return -1;
        }
        if(alnummer >= NUMAL) {
            cout << "Het getal is te groot!" << endl;
            return -1;
        }

        String trackerType = trackers[alnummer];
        cout << "Gebruik tracker type " << trackerType << endl;
        if (trackerType == "BOOSTING")
            tracker = TrackerBoosting::create();
        if (trackerType == "MIL")
            tracker = TrackerMIL::create();
        if (trackerType == "KCF")
            tracker = TrackerKCF::create();
        if (trackerType == "TLD")
            tracker = TrackerTLD::create();
        if (trackerType == "MEDIANFLOW")
            tracker = TrackerMedianFlow::create();
        if (trackerType == "GOTURN")
            tracker = TrackerGOTURN::create();
        if (trackerType == "MOSSE")
            tracker = TrackerMOSSE::create();
        if (trackerType == "CSRT")
            tracker = TrackerCSRT::create();

        //Tracker + window aanmaken
        cout << "Video frame per frame inlezen en nieuwe bbox berekenen..." << endl;
        tracker->init(frame, bbox);
        namedWindow("Tracking");
        imshow("Tracking", frame);

        while(video.read(frame)) {
            //Tracker updaten
            ok = tracker->update(frame, bbox);

            //Lukt het tracken?
            if(ok) {
                //Gelukt! -> bbox weergeven op nieuw frame
                rectangle(frame, bbox, Scalar(255, 0, 0), 2, 1);
            } else {
                cerr << "TRACKEN LUKT NIET MEER!!" << endl;
            }

            //Nieuw frame weergeven
            imshow("Tracking", frame);
            waitKey(1);
        }
    }
}
