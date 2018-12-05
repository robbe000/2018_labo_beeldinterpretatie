#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/ml.hpp"
#include <string.h>

using namespace std;
using namespace cv;
using namespace ml;

Mat input_glob;
Mat input_glob_hsv;
vector<Point> voorgrond, achtergrond;
int status;     //0 voorgrond detectren, 1 achtergrond detecteren

void mCallback(int event, int x, int y, int flags, void* userdata) {
    Vec3b hsv = input_glob_hsv.at<Vec3b>(x,y);

    if(event == EVENT_LBUTTONDOWN) {
        cout << "Links geklikt \t Pos: "<< x << " " << y << "\n";
        cout << "Pixel waarde: H:" << (int)hsv.val[0] << " S:" << (int)hsv.val[1] << " V:" << (int)hsv.val[2] << "\t";

        if(status == 0) {
            voorgrond.push_back(Point(x, y));
            cout << "Aantal punten voorgrond: " << voorgrond.size();
        } else if (status == 1) {
            achtergrond.push_back(Point(x, y));
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
    cvtColor(input_img, input_glob_hsv, CV_BGR2HSV);

    namedWindow("Beeld");
    moveWindow("Beeld", 800, 0);

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

    cout << "Trainingsdata genereren..." << endl;

    Mat train_voorgrond, train_achtergrond;
    Mat class_voorgrond, class_achtergrond;
    Mat class_tot, train_tot;

    train_voorgrond.create(voorgrond.size(), 3, CV_32FC1);
    train_achtergrond.create(achtergrond.size(), 3, CV_32FC1);

    class_voorgrond.create(voorgrond.size(), 1, CV_32SC1);
    class_achtergrond.create(achtergrond.size(), 1, CV_32SC1);

    //Voorgrond trainingsdata vormen
    for(uint i=0; i<voorgrond.size(); i++) {
        train_voorgrond.at<float>(i, 0) = input_glob_hsv.at<Vec3b>(voorgrond[i])[0];
        train_voorgrond.at<float>(i, 1) = input_glob_hsv.at<Vec3b>(voorgrond[i])[1];
        train_voorgrond.at<float>(i, 2) = input_glob_hsv.at<Vec3b>(voorgrond[i])[2];
    }

    //Voorgrond class op 1 => is goed
    class_voorgrond = Mat::ones(class_voorgrond.rows, class_voorgrond.cols, CV_32SC1);


    //Achtergrond trainingsdata vormen
    for(uint i=0; i<achtergrond.size(); i++) {
        train_achtergrond.at<float>(i, 0) = input_glob_hsv.at<Vec3b>(voorgrond[i])[0];
        train_achtergrond.at<float>(i, 1) = input_glob_hsv.at<Vec3b>(voorgrond[i])[1];
        train_achtergrond.at<float>(i, 2) = input_glob_hsv.at<Vec3b>(voorgrond[i])[2];
    }

    //Achtergrond class op 0 => niet goed
    class_achtergrond = Mat::zeros(class_achtergrond.rows, class_achtergrond.cols, CV_32SC1);

    //Samenvoegen
    vconcat(train_voorgrond, train_achtergrond, train_tot);
    vconcat(class_voorgrond, class_achtergrond, class_tot);

    //Waarden afprinten der controle
    cout << "--" << endl;
    for(int i=0;i<train_tot.rows;i++) {
        cout << (float)train_tot.at<float>(i, 0);
        cout << " ";
        cout << (float)train_tot.at<float>(i, 1);
        cout << " ";
        cout << (float)train_tot.at<float>(i, 2);
        cout << " ";
        cout << (int)class_tot.at<uchar>(i, 0);
        cout << endl;
    }
    cout << "--" << endl;

    //KNN
    cout << "K-Nearest-Neighbor trainen..." << endl;

    Ptr<KNearest> knn = KNearest::create();
    Ptr<TrainData> trainKnn = TrainData::create(train_tot, ROW_SAMPLE, class_tot);

    knn->setIsClassifier(true);
    knn->setAlgorithmType(KNearest::BRUTE_FORCE);
    knn->setDefaultK(3);
    knn->train(trainKnn);


    //NBC
    cout << "Normal Bayes Classifier trainen..." << endl;

    Ptr<NormalBayesClassifier> nbc = NormalBayesClassifier::create();
    nbc->train(train_tot, ROW_SAMPLE, class_tot);


    //SVM
    cout << "Support Vector Machine trainen..." << endl;

    Ptr<SVM> svm = SVM::create();
    svm->setType(SVM::C_SVC);
    svm->setKernel(SVM::LINEAR);
    svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));
    svm->train(train_tot, ROW_SAMPLE, class_tot);


    //Berekenen op inputafbeelding
    cout << "Uitvoeren op input afbeelding..." << endl;

    Mat class_knn, class_nbc, class_svm;
    Mat result_knn, result_nbc, result_svm;

    result_knn = Mat::zeros(input_img.rows, input_img.cols, CV_8UC1);
    result_nbc = Mat::zeros(input_img.rows, input_img.cols, CV_8UC1);
    result_svm = Mat::zeros(input_img.rows, input_img.cols, CV_8UC1);

    for(int x=0; x<input_img.rows; x++) {
        for(int y=0; y<input_img.cols; y++) {
            //Input voor test genereren
            Vec3b hsv = input_glob_hsv.at<Vec3b>(x,y);
            Mat input_pixel(1, 3, CV_32FC1);
            input_pixel.at<float>(0, 0) = hsv[0];
            input_pixel.at<float>(0, 1) = hsv[1];
            input_pixel.at<float>(0, 2) = hsv[2];

            knn->findNearest(input_pixel, knn->getDefaultK(), class_knn);
            nbc->predict(input_pixel, class_nbc);
            svm->predict(input_pixel, class_svm);

            result_knn.at<uchar>(x, y) = (uchar)class_knn.at<float>(0,0);
            result_nbc.at<uchar>(x, y) = (uchar)class_nbc.at<int>(0,0);
            result_svm.at<uchar>(x, y) = (uchar)class_svm.at<float>(0,0);
        }
    }

    //Binaire afbeelding omvormen naar een zichtbaar masker
    result_knn = result_knn*255;
    result_nbc = result_nbc*255;
    result_svm = result_svm*255;

    namedWindow("knn");
    imshow("knn", result_knn);

    namedWindow("nbc");
    imshow("nbc", result_nbc);

    namedWindow("svm");
    imshow("svm", result_svm);

    waitKey(0);

}
