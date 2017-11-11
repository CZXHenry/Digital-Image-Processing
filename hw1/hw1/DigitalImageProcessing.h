#include <iostream>
#include <math.h>
#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

//储存图像大小的结构体
struct ImageSize
{
	int width;
	int height;
	ImageSize()
	{
		width = 0;
		height = 0;
	}
	ImageSize(int mwidth, int mheight)
	{
		width = mwidth;
		height = mheight;
	}
};

//用来处理数字图像的类
class DigitalImageProcessing
{
public:
	Mat Scaling(Mat, struct ImageSize);
	Mat Quantization(Mat, int level);
	Mat EqualizeHist(Mat InputImage);
	Mat Filter2d(Mat,size_t row,size_t col, double* filter[]);
};