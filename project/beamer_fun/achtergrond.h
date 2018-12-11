#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/ml.hpp"
#include <string.h>

#define STAT_WAIT_SCHERMDET 0
#define STAT_WAIT_HANDDET 1
#define STAT_HAND_TRACK 2
#define STAT_DETECT_SCHERM 3

using namespace std;
using namespace cv;
using namespace ml;

typedef struct Toets {
    String letter;
    Point positieLB;
    Point positieRB;
} Toets;

static vector<Toets> toetsen;
static String zin = "";
static vector<char> geschiedenis;

Mat tekenAchtergrond(int status);
void tekenBoodschap(Mat achtergrond, String text);
Mat tekenTracker(int status, Rect2d positie, float schaal, Mat frame);
Rect2d berekenPos(Rect2d positie, float schaal, Mat frame);
Mat tekenToetsenbord(Rect2d positie, Mat achtergrond);
void berekenToetsen(void);
