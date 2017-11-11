#include "DigitalImageProcessing.h"

Mat DigitalImageProcessing::Scaling(Mat InputImage, ImageSize OutputImageSize)
{
	struct ImageSize InputImageSize;
	int InputImageChannels;
	double HeightScale;//高度的缩放比例
	double WidthScale;//宽度的缩放比例
	InputImageSize.height = InputImage.rows;
	InputImageSize.width = InputImage.cols;

	//计算高和宽的缩放比例
	HeightScale = (double)InputImageSize.height / (double)OutputImageSize.height;
	WidthScale = (double)InputImageSize.width / (double)OutputImageSize.width;

	//设定输出图像的像素矩阵
	//CV_8UC1参数表明该矩阵是利用unsigned char来储存像素值
	//即每个像素的值是用一个字节（一个unsigned char的大小表示）
	Mat OutputImage(OutputImageSize.height, OutputImageSize.width, CV_8UC1);
	//设定输入和输出图片的矩阵指针，用于操作矩阵中的像素值
	uchar *InputPtr = InputImage.data;
	uchar *OutputPtr = OutputImage.data;
	double x, y;
	int x1, x2, y1, y2;
	//遍历输出图像中的每一个像素，通过双线性插值的方法来找到新图像像素点的像素值
	//i表示新图像中像素坐标的y值，j表示坐标的x值
	//因为在Mat数据结构一般是将所有像素值储存在一行中的，而不是储存在一个二维的数组中的
	//所以要取得某个特定坐标的像素值还要经过计算才行
	for (int i = 0; i < OutputImageSize.height; i++)
	{
		for (int j = 0; j < OutputImageSize.width; j++)
		{
			//计算新图像坐标对应的原图像坐标
			x = j*WidthScale;
			y = i*HeightScale;
			//获得最邻近的四个像素点的坐标
			x1 = floor(x);
			x2 = ceil(x);
			y1 = floor(y);
			y2 = ceil(y);
			//按情况设定x1，x2，y1，y2
			//当x1，x2，y1，y2均为整数时，直接将原图像像素值赋给新图像的对应像素
			if (x1 == x2&&y1 == y2)
			{
				OutputPtr[i*OutputImageSize.width + j] = InputPtr[x1 + y1*InputImageSize.width];

			}
			else
			{
				//如果x1位于原图像最右边一列时，要防止超出宽度范围
				if (x1 == (InputImageSize.width - 1))
				{
					x2 = x1 - 1;
				}
				//如果y1位于原图像最右边一列时，要防止超出高度范围
				else if (y1 == (InputImageSize.height - 1))
				{
					y2 = y1 - 1;
				}
				//当y为整数时
				else if (x1 != x2&&y1 == y2)
				{
					y2++;
				}
				//当x为整数
				else if(x1==x2&&y1!=y2)
				{
					x2++;
				}
				//利用双线性插值公式计算新图像中像素的像素值
				OutputPtr[i*OutputImageSize.width + j] =
				(
							InputPtr[x1 + y1*InputImageSize.width] * (x2 - x)*(y2 - y) +
							InputPtr[x2 + y1*InputImageSize.width] * (x - x1)*(y2 - y) +
							InputPtr[x1 + y2*InputImageSize.width] * (x2 - x)*(y - y1) +
							InputPtr[x2 + y2*InputImageSize.width] * (x - x1)*(y - y1)
				) / ((x2 - x1)*(y2 - y1));
			} 

		}
	}
	//返回新图像的像素矩阵
	return OutputImage;
}

Mat DigitalImageProcessing::Quantization(Mat InputImage, int level)
{
	//求出灰度范围
	int LevelWidth = 256 / level;
	Mat OutputImage(InputImage.rows, InputImage.cols, CV_8UC1);
	uchar *InputPtr = InputImage.data;
	uchar *OutputPtr = OutputImage.data;
	//遍历原图像中的所有像素，根据像素值所在范围设定新的灰度值
	//将新的灰度值赋给新图像对应的像素
	for (int i = 0; i < InputImage.rows; i++)
	{
		for (int j = 0; j < InputImage.cols; j++)
		{
			OutputPtr[i*OutputImage.cols + j] = ((InputPtr[i*InputImage.cols + j]) / LevelWidth+1)*LevelWidth;
		}
	}
	//返回新图像的像素矩阵
	return OutputImage;
}

Mat DigitalImageProcessing::EqualizeHist(Mat InputImage)
{
	int InputImageHist[256] = { 0 };
	double L = 255;
	double coefficient = L / (InputImage.cols*InputImage.rows);//（L-1）/MN
	int SumTable[256] = { 0 };//储存每个像素值的离散累积概率分布的值
	Mat OutputImage(InputImage.rows, InputImage.cols, CV_8UC1);
	uchar* InputPtr = InputImage.data;
	uchar* OutputPtr = OutputImage.data;
	//统计各个灰度值的像素值数量
	for (int i = 0; i < InputImage.rows; i++)
	{
		for (int j = 0; j < InputImage.cols; j++)
		{
			InputImageHist[InputPtr[i*InputImage.cols + j]]++;
		}
	}

	SumTable[0] = InputImageHist[0];
	for (int i = 1; i < 256; i++)
	{
		SumTable[i] = SumTable[i - 1] + InputImageHist[i];
	}

	int temp = InputImageHist[0];
	for (int i = 0; i < InputImage.rows; i = i++)
	{
		for (int j = 0; j < InputImage.cols; j++)
		{
			int PixelValue = InputPtr[i*InputImage.cols + j];
			int value = coefficient*SumTable[PixelValue];
			OutputPtr[i*InputImage.cols + j] = value;
		}
	}
	return OutputImage;
}

Mat DigitalImageProcessing::Filter2d(Mat InputImage, size_t row,size_t col,double* filter[])
{
	Mat OutputImage(InputImage.rows, InputImage.cols, CV_8UC1);
	//Mat TempImage(InputImage.rows+(row-1)*2, InputImage.cols+(col-1)*2,CV_8UC1);
	vector<int> TempPtr;
	uchar* InputPtr = InputImage.data;
	uchar* OutputPtr = OutputImage.data;
	int TempImageRows = InputImage.rows + (row - 1) * 2;
	int TempImageCols = InputImage.cols + (col - 1) * 2;
	//边界补充零
	for (int i = 0; i < TempImageRows*TempImageCols; i++)
	{
		TempPtr.push_back(0);
	}
	for (int i = row - 1; i <(InputImage.rows + row - 1); i++)
	{
		for (int j = col - 1; j < (InputImage.cols + col - 1); j++)
		{
			TempPtr[i*TempImageCols + j] = InputPtr[(i - row - 1)*InputImage.cols + (j - col - 1)];
		}
	}

	//卷积操作
	for (int i = row - 1; i <= (InputImage.rows + row - 1); i++)
	{
		for (int j = col - 1; j <= (InputImage.cols + col - 1); j++)
		{
			cout << i << " " << j << endl;
			int CentralPos=(row+1)/2-1;//滤波器矩阵的中心点坐标
			int a = (row - 1) / 2;
			int b = a;
			int Sum=0;
			for (int s = -a; s <= a; s++)
			{
				for (int t = -b; t <= b; t++)
				{
					Sum = Sum + filter[CentralPos + s][CentralPos + t] * TempPtr[(i - s)*TempImageCols + (j - t)];
				}
			}
			if (Sum < 0)
			{
				Sum = 0;
			}
			else if (Sum > 255)
			{
				Sum = 255;
			}
			TempPtr[i*TempImageCols + j] = Sum;
		}
	}

	//对卷积计算之后获得的结果矩阵进行裁剪
	for (int i = row - 1; i < (InputImage.rows + row - 1); i++)
	{
		for (int j = col - 1; j < (InputImage.cols + col - 1); j++)
		{
			 OutputPtr[(i - row - 1)*InputImage.cols + (j - col - 1)]= TempPtr[i*TempImageCols + j];
		}
	}
	return OutputImage;
}

