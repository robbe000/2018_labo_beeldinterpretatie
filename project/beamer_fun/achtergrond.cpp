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

#define BREEDTE 1000
#define HOOGTE 600
#define TOETSENBORD_PADDING_X 200
#define TOETS_SPATIE 5
#define LIJN1_Y 200
#define MINPRESS 30

using namespace std;
using namespace cv;
using namespace ml;

Mat tekenAchtergrond(int status) {
    Mat achtergrond;
    achtergrond = Mat::zeros(HOOGTE, BREEDTE, CV_8UC3 );

    //Heel de achtergrond op groen zetten
    for(int c=0; c<achtergrond.cols; c++) {
        for(int r=0; r<achtergrond.rows; r++) {
            if(status == STAT_DETECT_SCHERM) {
                //Knalgroen voor detectie
                achtergrond.at<Vec3b>(r,c)[0] = 0;
                achtergrond.at<Vec3b>(r,c)[1] = 255;
                achtergrond.at<Vec3b>(r,c)[2] = 0;
            } else {
                achtergrond.at<Vec3b>(r,c)[0] = 100;
                achtergrond.at<Vec3b>(r,c)[1] = 200;
                achtergrond.at<Vec3b>(r,c)[2] = 100;
            }
        }
    }

    if(status == STAT_WAIT_HANDDET) {
        tekenBoodschap(achtergrond, "HAND DETECTEREN...");
    }

    if(status == STAT_HAND_TRACK) {
        tekenBoodschap(achtergrond, "HAND TRACKEN...");
    }

    return achtergrond;
}

Mat tekenTracker(int status, Rect2d positie, float schaal, Mat frame) {
    Mat achtergrond = tekenAchtergrond(status);

    positie = berekenPos(positie, schaal, frame);

    achtergrond = tekenToetsenbord(positie, achtergrond);

    rectangle(achtergrond, Point(positie.x+(positie.width/2)-10, positie.y+(positie.height/2)-10), Point(positie.x+(positie.width/2)+10, positie.y+(positie.height/2)+10), Scalar(255, 0, 0), 2, 1);

    return achtergrond;
}

void tekenBoodschap(Mat achtergrond, String text) {
    putText(achtergrond, text, Point(40, 40), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 255), 2, LINE_AA);
}

Rect2d berekenPos(Rect2d positie, float schaal, Mat frame) {
    float xposFrame = (float)positie.x/frame.cols;
    float yposFrame = (float)positie.y/frame.rows;

    positie.x = xposFrame * BREEDTE;
    positie.y = yposFrame * HOOGTE;

    return positie;
}

Mat tekenToetsenbord(Rect2d positie, Mat achtergrond){
    Scalar kleur;
    bool ingedrukt = false;
    Point midden = Point(positie.x+(positie.width/2), positie.y+(positie.height/2));
    for(uint i=0; i<toetsen.size(); i++) {
        if((midden.x > toetsen.at(i).positieLB.x) & (midden.x < toetsen.at(i).positieRB.x) & (midden.y > toetsen.at(i).positieLB.y) & (midden.y < toetsen.at(i).positieRB.y)) {
            //Op deze toets
            kleur = Scalar(255, 255, 255);
            //Wanneer een toets ingedrukt -> opslaan in geschidening
            geschiedenis.push_back(toetsen.at(i).letter[0]);
            ingedrukt=true;
        } else {
            //Niet op deze toets
            kleur = Scalar(100, 255, 100);
        }
        //De toets tekenen
        rectangle(achtergrond, toetsen.at(i).positieLB, toetsen.at(i).positieRB, kleur, 1);
        putText(achtergrond, toetsen.at(i).letter, Point(toetsen.at(i).positieLB.x + (toetsen.at(i).positieRB.x - toetsen.at(i).positieLB.x)/2, toetsen.at(i).positieLB.y + (toetsen.at(i).positieRB.y - toetsen.at(i).positieLB.y)/2), CV_FONT_HERSHEY_SIMPLEX, 1, kleur, 2, LINE_AA);
    }

    //Wanneer iets werd toegevoegd aan de geschiedenis bekijken of de knop lang genoeg werd ingedrukt (MINPRESS)
    if((geschiedenis.size()>0) & (ingedrukt == true)) {
        char vorige = geschiedenis.at(geschiedenis.size()-1);
        int teller=0;
        for(uint i = geschiedenis.size()-1; i>0; i--) {
            if(vorige != geschiedenis.at(i)) {
                //Een verandering waargenomen!
                break;
            }
            teller++;
            vorige = geschiedenis.at(i);
        }

        cout << teller << endl;

        for(uint j=0; j<geschiedenis.size(); j++) {
            cout << geschiedenis.at(j) << " ";
        }
        cout << endl;

        if(teller > MINPRESS) {
            //Gedrukt!
            cout << "GEDRUKT!" << endl;
            zin += vorige;

            //geschiedenis leegmaken
            geschiedenis.clear();
        }
    }

    //De ingegeven zin afdrukken
    putText(achtergrond, zin, Point(40, 80), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 255), 2, LINE_AA);

    return achtergrond;
}

void berekenToetsen(void) {
    String lijn1[10] = {"A", "Z", "E", "R", "T", "Y", "U", "I", "O", "P"};
    String lijn2[10] = {"Q", "S", "D", "F", "G", "H", "I", "K", "L", "M"};
    String lijn3[7] = {"W", "X", "C", "V", "B", "N", " "};

    //1e lijn invullen
    int toestsBreedte = (BREEDTE - (TOETSENBORD_PADDING_X*2)) / 10 ;
    for(int i=0; i<10; i++) {
        Toets buffer;
        buffer.letter = lijn1[i];
        buffer.positieLB = Point(toestsBreedte*i+TOETSENBORD_PADDING_X, LIJN1_Y);
        buffer.positieRB = Point(toestsBreedte*(i+1)-TOETS_SPATIE+TOETSENBORD_PADDING_X, LIJN1_Y+toestsBreedte-TOETS_SPATIE);
        toetsen.push_back(buffer);
    }

    //2e lijn invullen
    for(int i=0; i<10; i++) {
        Toets buffer;
        buffer.letter = lijn2[i];
        buffer.positieLB = Point(toestsBreedte*i+TOETSENBORD_PADDING_X, LIJN1_Y + (toestsBreedte + TOETS_SPATIE));
        buffer.positieRB = Point(toestsBreedte*(i+1)-TOETS_SPATIE+TOETSENBORD_PADDING_X, LIJN1_Y+toestsBreedte-TOETS_SPATIE + (toestsBreedte + TOETS_SPATIE));
        toetsen.push_back(buffer);
    }

    //3e lijn invullen
    for(int i=0; i<7; i++) {
        Toets buffer;
        buffer.letter = lijn3[i];
        buffer.positieLB = Point(toestsBreedte*i+TOETSENBORD_PADDING_X, LIJN1_Y + (toestsBreedte + TOETS_SPATIE)*2);
        buffer.positieRB = Point(toestsBreedte*(i+1)-TOETS_SPATIE+TOETSENBORD_PADDING_X, LIJN1_Y+toestsBreedte-TOETS_SPATIE + (toestsBreedte + TOETS_SPATIE)*2);
        toetsen.push_back(buffer);
    }
}
