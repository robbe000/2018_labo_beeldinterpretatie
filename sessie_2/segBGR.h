#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void segBGR(Mat imageIn[]);
void voerUit(void);

//Slider
void sliderRood( int, void* );
void sliderRoodMin(int, void*);
