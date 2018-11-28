#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <string.h>
#include "match.h"

#define MIN_DIST 1000
#define MAX_DIST 0
#define MULBAD 3

using namespace std;
using namespace cv;

//Een helpende bron:
//https://docs.opencv.org/3.0-beta/doc/tutorials/features2d/akaze_matching/akaze_matching.html
//Info ivm RANSAC:
//https://docs.opencv.org/3.4/d7/dff/tutorial_feature_homography.html


void match(Mat input_img, Mat template_img) {
    vector<KeyPoint> kpts1, kpts2;
    Mat desc1, desc2;

    Ptr<ORB> kdet = ORB::create();

    //De keypoints detecteren en descriptoren berekenen
    kdet->detectAndCompute(input_img, noArray(), kpts1, desc1);
    kdet->detectAndCompute(input_img, noArray(), kpts2, desc2);

    //Op de twee afbeeldingen de keypoints matchen
    BFMatcher matcher(NORM_HAMMING);
    vector<vector<DMatch>> nn_matches;
    matcher.knnMatch(desc1, desc2, nn_matches, 2);

    //De gevonden matches tekenen
    Mat img_rawmatch;
    drawMatches(input_img, kpts1, template_img, kpts2, nn_matches, img_rawmatch);
    namedWindow("RawMatches");
    imshow("RawMatches", img_rawmatch);
    waitKey(0);

    //Veel, veel, veel te veel matches
    //(Vanaf hier het voorbeeld wel vereenvoudigd omdat ze het daar veel te ingewikkeld maken!)
    //--> In de eerste plaats filteren op min en max afstand
    double min_dist = MIN_DIST;
    double max_dist = MAX_DIST;
    vector<DMatch> good_matches;

    //Slechtste en beste match bepalen
    for(int i=0; i<desc1.rows; i++) {
        double dist = nn_matches[i][0].distance;
        if(dist > max_dist) {
            max_dist = dist;
        }
        if(dist < min_dist) {
            min_dist = dist;
        }
    }

    //De slechtste matches verwijderen
    for(int i=0; i<desc1.rows; i++) {
        if(nn_matches[i][0].distance <= 3*min_dist) {
            good_matches.push_back(nn_matches[i][0]);
        }
    }

    //De al betere matches weergeven
    Mat img_match;
    drawMatches(input_img, kpts1, template_img, kpts2, good_matches, img_match);
    namedWindow("Matches");
    imshow("Matches", img_match);
    waitKey(0);

    //Dit is al een pak beter, nu ransac toepassen
    vector<Point2f> input, templ;

    //Keypoints van de goede matches verzamelen
    for(size_t i=0; i< good_matches.size(); i++) {
        input.push_back(kpts1[good_matches[i].queryIdx].pt);
        templ.push_back(kpts2[good_matches[i].queryIdx].pt);
    }

    Mat H = findHomography(templ, input, RANSAC);

    //De hoeken van het te zoeken object zoeken
    vector<Point2f> obj_corners(4);
    vector<Point2f> scene_corners(4);

    obj_corners[0] = Point2f(0, 0);
    obj_corners[1] = Point2f((float)template_img.cols, 0);
    obj_corners[2] = Point2f((float)template_img.cols, (float)template_img.rows);
    obj_corners[3] = Point2f(0, (float)template_img.rows);

    perspectiveTransform(obj_corners, scene_corners, H);

    line( img_match, scene_corners[0] + Point2f((float)template_img.cols, 0), scene_corners[1] + Point2f((float)template_img.cols, 0), Scalar(0, 255, 0), 4 );
    line( img_match, scene_corners[1] + Point2f((float)template_img.cols, 0), scene_corners[2] + Point2f((float)template_img.cols, 0), Scalar( 0, 255, 0), 4 );
    line( img_match, scene_corners[2] + Point2f((float)template_img.cols, 0), scene_corners[3] + Point2f((float)template_img.cols, 0), Scalar( 0, 255, 0), 4 );
    line( img_match, scene_corners[3] + Point2f((float)template_img.cols, 0), scene_corners[0] + Point2f((float)template_img.cols, 0), Scalar( 0, 255, 0), 4 );

    namedWindow("Final Result");
    imshow("Final Result", img_match);
    waitKey(0);
}
