////////////////////////////////////////////////////////////////////////////
//
//   Project     : Beamer Fun
//   Description :
//
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

#include "achtergrond.h"
#include "detection.h"
#include "detect_scherm.h"

#define HANDMODEL "Hand.Cascade.1.xml"
#define MAXSCHAAL 3

using namespace std;
using namespace cv;
using namespace ml;

int main(int argc, const char** argv) {
    VideoCapture vcap;
    Mat frame;
    int status = STAT_DETECT_SCHERM;
    float schaal = MAXSCHAAL + 1;
    Rect scherm;
    Rect2d positie;
    int detectTeller = 0;

    //Helpboodschap configureren
    CommandLineParser parser(argc, argv,
    "{help | |Voorzie een live video}");

    //Indien gevraagd helpboodschap afdrukken
    if(parser.has("help")) {
        parser.printMessage();
        return -1;
    }

    //Model inlezen
    CascadeClassifier haar_face_classifier;

    if(!haar_face_classifier.load(HANDMODEL)) {
        cout << "Kon " << HANDMODEL << " niet inlezen!" << endl;
        return -1;
    }

    //Tracker configureren
    Ptr<Tracker> tracker;
    tracker = TrackerBoosting::create();

    //Positie van de toetsen berekenen
    berekenToetsen();

    //Live video canvas klaarzetten
    namedWindow("Live");

    //Begin achtergrond maken + weergeven
    namedWindow("Achtergrond");
    imshow("Achtergrond", tekenAchtergrond(status));

    waitKey(0);

    //Videostream van IPCamera (GSM)
    if(!vcap.open("http://192.168.1.111:8080/video")) {
        cout << "Kon videostream niet openen!" << endl;
        return -1;
    }
    cout << "Videostream gevonden!" << endl;

    while(1) {
        if(!vcap.read(frame)) {
            cout << "Kon frame niet inlezen" << endl;
            return -1;
        }

        //Een scherm gevonden?
        //Of scherm te ver van camera?
        //Verhouding inputbeeld/scherm mag maximum MAXSCHAAL zijn
        if(status == STAT_DETECT_SCHERM) {
            scherm = detect_scherm(frame);
            schaal = (float)frame.cols/scherm.width;
            cout << "Verhouding: " << schaal << endl;

            if(schaal < MAXSCHAAL) {
                status = STAT_WAIT_HANDDET;
                imshow("Achtergrond", tekenAchtergrond(status));
            }
        }

        //Om het hand te kunnen tracken moet er natuurlijk wel eerst een hand gevonden worden!
        if(status == STAT_WAIT_HANDDET) {
            //Voor snelheid slechts 1 keer om de 20 frames zoeken naar een hand
            if(detectTeller == 20) {
                positie = detect(frame, haar_face_classifier);
                detectTeller=0;
                cout << positie << endl;
                //Een nieuw hand gevonden?
                if((positie.x != 0) && (positie.y != 0)) {
                    cout << "Gevonden" << endl;

                    //Bbox kleiner maken voor betere tracking
                    /*positie.x = positie.x + positie.width/4;
                    positie.width = positie.width/2;
                    positie.y = positie.y + positie.height/4;
                    positie.height = positie.height/2;*/

                    //Tracker klaarmaken
                    tracker->init(frame, positie);

                    //Achtergrond aanpassen
                    status = STAT_HAND_TRACK;
                    imshow("Achtergrond", tekenAchtergrond(status));

                    //Videostream van IPCamera opnieuw openen (GSM)
                    if(!vcap.open("http://192.168.1.111:8080/video")) {
                        cout << "Kon videostream niet openen!" << endl;
                        return -1;
                    }
                }
                imshow("Live", frame);
            }
            detectTeller++;
        }

        //Het hand tracken
        if(status == STAT_HAND_TRACK) {
            //Tracker updaten
            bool ok = tracker->update(frame, positie);

            //Lukt het tracken?
            if(ok) {
                //Gelukt! -> bbox weergeven op nieuw frame
                rectangle(frame, positie, Scalar(255, 0, 0), 2, 1);
            } else {
                cerr << "TRACKEN LUKT NIET MEER!!" << endl;
            }
            imshow("Live", frame);
            imshow("Achtergrond", tekenTracker(status, positie, schaal, frame));
        }

        //imshow("Live", frame);
        waitKey(3);
    }
}
