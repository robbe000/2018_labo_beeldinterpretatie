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

using namespace std;
using namespace cv;
using namespace ml;

Mat tekenAchtergrond(int status);
void tekenBoodschap(Mat achtergrond, String text);
Mat tekenTracker(int status, Rect2d positie);
