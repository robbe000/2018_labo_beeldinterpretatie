#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, const char** argv) {
    cout << "Hello world!" << endl;

    //Helpboodschap configureren
    CommandLineParser parser(argc, argv,
    "{help | |Een boodschap}"
    "{img1 | |Het absolute pat van de eerste afbeelding}");

    if(parser.has("help")) {
        parser.printMessage();
        cerr << "TIP: geef het absolute pat weer!" << endl;
        return -1;
    }

    string img1(parser.get<string>("img1"));

    return 0;
}


