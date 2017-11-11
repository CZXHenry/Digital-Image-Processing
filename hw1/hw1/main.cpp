#define _CRT_SECURE_NO_WARNINGS

#include <iostream>  
#include <string>
#include <stdio.h>
#include <Windows.h>
#include <fstream>
#include <opencv2\opencv.hpp>
#include "DigitalImageProcessing.h"

using namespace cv;
using namespace std;

const string FilePath = "44.png";
const string FileName = "44";

int UI();
struct ImageSize GetSize();
int GetGreyLevel();
void CreateRequiredImage(Mat InputImage);

int main()
{
	double **filter=new double*[3];
	for (int i = 0; i < 3; i++)
	{
		filter[i] = new double[3]{ 1,1,1 };
	}
	Mat InputImage = imread("44.png", IMREAD_GRAYSCALE);
	DigitalImageProcessing test;
	Mat OutputImage = test.Filter2d(InputImage, 3, 3, filter);
	imwrite("44_output.png", OutputImage);
	return 0;
}

/*int main()
{
	Mat InputImage=imread(FilePath, IMREAD_GRAYSCALE);
	Mat OutputImage;
	DigitalImageProcessing Test;
	char *OutputFileName=new char;


	char isQuit;
	while (1)
	{
		switch (UI()) 
		{
		case 1:
		{
			OutputImage = Test.Scaling(InputImage, GetSize());
			sprintf(OutputFileName, (FileName +"_%dx%d.png").c_str(), OutputImage.cols, OutputImage.rows);
			break;
		}
		case 2:
		{
			int GreyLevel = GetGreyLevel();
			OutputImage = Test.Quantization(InputImage, GreyLevel);
			sprintf(OutputFileName, (FileName +"_GreyLevel_%d.png").c_str(), GreyLevel);
			break;
		}
		case 3:
		{
			CreateRequiredImage(InputImage);
			cout << "全部图片已生成，请到项目目录下查看" << endl;
			break;
		}
		}
		if (!OutputImage.empty())
		{
			imwrite(OutputFileName, OutputImage);
		}
		cout << "按任意键返回上一层，按Q退出" << endl;
		cin >> isQuit;
		if (isQuit == 'Q')
		{
			break;
		}
		else
		{
			system("cls");
		}
		fflush(stdin);
	}
	return 0;
}

int UI()
{
	cout << "数字图像处理系统（简陋版）" << endl;
	cout << "-------------------------" << endl;
	cout << "1.缩放" << endl;
	cout << "2.量化" << endl;
	cout << "3.生成所有项目要求图片" << endl;
	cout << "请输入功能前的序号（均使用44.png进行操作演示）" << endl;
	int chose;
	cin >> chose;
	return chose;
}

struct ImageSize GetSize()
{
	int width, height;
	cout << "请输入图像的放缩到的长和宽" << endl;
	cout << "width : ";
	cin >> width;
	cout << endl << "height : ";
	cin >> height;
	return ImageSize(width, height);
}

int GetGreyLevel()
{
	int GreyLevel;
	cout << "请输入灰度级别 : ";
	cin >> GreyLevel;
	return GreyLevel;
}

void CreateRequiredImage(Mat InputImage)
{
	DigitalImageProcessing Test;
	Mat OutputImage;
	OutputImage = Test.Scaling(InputImage, struct ImageSize(192, 128));
	imwrite("44_192x128.png", OutputImage);
	OutputImage = Test.Scaling(InputImage, struct ImageSize(96, 64));
	imwrite("44_96x64.png", OutputImage);
	OutputImage = Test.Scaling(InputImage, struct ImageSize(48, 32));
	imwrite("44_48x32.png", OutputImage);
	OutputImage = Test.Scaling(InputImage, struct ImageSize(24, 16));
	imwrite("44_24x16.png", OutputImage);
	OutputImage = Test.Scaling(InputImage, struct ImageSize(12, 8));
	imwrite("44_12x8.png", OutputImage);
	OutputImage = Test.Scaling(InputImage, struct ImageSize(300, 200));
	imwrite("44_300x200.png", OutputImage);
	OutputImage = Test.Scaling(InputImage, struct ImageSize(450, 300));
	imwrite("44_450x300.png", OutputImage);
	OutputImage = Test.Scaling(InputImage, struct ImageSize(500, 200));
	imwrite("44_500x200.png", OutputImage);
	OutputImage = Test.Quantization(InputImage, 128);
	imwrite("44_128.png", OutputImage);
	OutputImage = Test.Quantization(InputImage, 32);
	imwrite("44_32.png", OutputImage);
	OutputImage = Test.Quantization(InputImage, 8);
	imwrite("44_8.png", OutputImage);
	OutputImage = Test.Quantization(InputImage, 4);
	imwrite("44_4.png", OutputImage);
	OutputImage = Test.Quantization(InputImage, 2);
	imwrite("44_2.png", OutputImage);
	OutputImage = Test.Scaling(InputImage, struct ImageSize(192, 128));
	imwrite("44_192*128.png", OutputImage);
}*/