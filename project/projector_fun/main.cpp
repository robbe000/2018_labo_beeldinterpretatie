#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/ml.hpp"
#include "opencv2/tracking.hpp"
#include "opencv2/core/ocl.hpp"
#include <string.h>

using namespace std;
using namespace cv;

int main(int argc, const char** argv) {
    //Introductie weergeven
    cout << "Project: Labo Beeldinterpretatie (Projector Fun)" << endl;
    cout << "In opdracht van: KU Leuven (Campus De Nayer)" << endl;
    cout << "Academiejaar: 2018-2019" << endl;
    cout << "Student: Robbe Goovaerts" << endl;

    //Helpboodschap configureren
    CommandLineParser parser(argc, argv,
    "{help | |Dit project heeft als doel het bedienen van bv. een spel met behulp van bewegingen die voor een projector uitgevoerd dienen te worden.}");

    //Indien gevraagd helpboodschap afdrukken
    if(parser.has("help")) {
        parser.printMessage();
        return -1;
    }


}
