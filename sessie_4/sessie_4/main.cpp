#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/highgui.hpp"
#include <string.h>

using namespace std;
using namespace cv;

vector<KeyPoint> features(Mat input, int soort) {
    vector<KeyPoint> keypoints;

    switch (soort) {
        case 0: {Ptr<ORB> detector = ORB::create();
                detector->detect(input, keypoints);
                break;}
        case 1: {Ptr<BRISK> detector = BRISK::create();
                detector->detect(input, keypoints);
                break;}
        case 2: {Ptr<AKAZE> detector = AKAZE::create();
                detector->detect(input, keypoints);
                break;}
    }

    return keypoints;
}

int main(int argc, const char** argv) {
    Mat input_img, template_img;

    //Helpboodschap configureren
    CommandLineParser parser(argc, argv,
    "{help | |Met dit programma dienen twee parameters meegegeven te worden}"
    "{input | |Het absolute pat van de eerste afbeelding (Input image)}"
    "{template | |Het absolute pat van de twede afbeelding (Template)}");

    //Indien gevraagd helpboodschap afdrukken
    if(parser.has("help")) {
        parser.printMessage();
        return -1;
    }

    //Parameters inlezen
    string input(parser.get<string>("input"));
    string template1(parser.get<string>("template"));

    //Juiste hoeveelheid gegevens ingegeven?
    if((input == "")|(template1 == "")) {
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

    //Afbeeldingen inlezen + nakijken of dit gelukt is
    cout << "File " << template1 << " inlezen..." << endl;
    template_img = imread(template1, CV_LOAD_IMAGE_COLOR);
    if(!template_img.data)
    {
        cout <<  "Kon template niet openen" << endl ;
        return -1;
    }

    namedWindow("Input");
    imshow("Input", input_img);
    namedWindow("Template");
    imshow("Template", template_img);
    //waitKey(0);

    //Keypoints zoeken
    vector<KeyPoint> kinput_orb;
    vector<KeyPoint> kinput_brisk;
    vector<KeyPoint> kinput_akaze;
    vector<KeyPoint> ktemplate_orb;
    vector<KeyPoint> ktemplate_brisk;
    vector<KeyPoint> ktemplate_akaze;

    Mat input_orb, input_brisk, input_akaze;
    Mat template_orb, template_brisk, template_akaze;

    input_orb = input_img.clone();
    input_brisk = input_img.clone();
    input_akaze = input_img.clone();

    template_orb = template_img.clone();
    template_brisk = template_img.clone();
    template_akaze = template_img.clone();

    kinput_orb = features(input_orb, 0);
    drawKeypoints(input_orb, kinput_orb, input_orb, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

    kinput_brisk = features(input_brisk, 1);
    drawKeypoints(input_brisk, kinput_brisk, input_brisk, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

    kinput_akaze = features(input_akaze, 2);
    drawKeypoints(input_akaze, kinput_akaze, input_akaze, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

    ktemplate_orb = features(template_orb, 0);
    drawKeypoints(template_orb, ktemplate_orb, template_orb, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

    ktemplate_brisk = features(template_brisk, 1);
    drawKeypoints(template_brisk, ktemplate_brisk, template_brisk, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

    ktemplate_akaze = features(template_akaze, 2);
    drawKeypoints(template_akaze, ktemplate_akaze, template_akaze, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

    namedWindow("input_orb");
    imshow("input_orb", input_orb);
    namedWindow("input_brisk");
    imshow("input_brisk", input_brisk);
    namedWindow("input_akaze");
    imshow("input_akaze", input_akaze);

    namedWindow("temp_orb");
    imshow("temp_orb", template_orb);
    namedWindow("temp_brisk");
    imshow("temp_brisk", template_brisk);
    namedWindow("temp_akaze");
    imshow("temp_akaze", template_akaze);
    waitKey(0);

    //Descriptors berekenen
    /*vector<DMatch> matches;
    Mat img_matches;
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::BRUTEFORCE);
    matcher->match(kinput_orb, kinput_brisk, matches);

    drawMatches(input_img, kinput_orb, template_img, ktemplate_orb, matches, img_matches);

    namedWindow("Matches");
    imshow("Matches", img_matches);*/

}
