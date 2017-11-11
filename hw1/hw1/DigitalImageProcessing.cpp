#include "DigitalImageProcessing.h"

Mat DigitalImageProcessing::Scaling(Mat InputImage, ImageSize OutputImageSize)
{
	struct ImageSize InputImageSize;
	int InputImageChannels;
	double HeightScale;//�߶ȵ����ű���
	double WidthScale;//��ȵ����ű���
	InputImageSize.height = InputImage.rows;
	InputImageSize.width = InputImage.cols;

	//����ߺͿ�����ű���
	HeightScale = (double)InputImageSize.height / (double)OutputImageSize.height;
	WidthScale = (double)InputImageSize.width / (double)OutputImageSize.width;

	//�趨���ͼ������ؾ���
	//CV_8UC1���������þ���������unsigned char����������ֵ
	//��ÿ�����ص�ֵ����һ���ֽڣ�һ��unsigned char�Ĵ�С��ʾ��
	Mat OutputImage(OutputImageSize.height, OutputImageSize.width, CV_8UC1);
	//�趨��������ͼƬ�ľ���ָ�룬���ڲ��������е�����ֵ
	uchar *InputPtr = InputImage.data;
	uchar *OutputPtr = OutputImage.data;
	double x, y;
	int x1, x2, y1, y2;
	//�������ͼ���е�ÿһ�����أ�ͨ��˫���Բ�ֵ�ķ������ҵ���ͼ�����ص������ֵ
	//i��ʾ��ͼ�������������yֵ��j��ʾ�����xֵ
	//��Ϊ��Mat���ݽṹһ���ǽ���������ֵ������һ���еģ������Ǵ�����һ����ά�������е�
	//����Ҫȡ��ĳ���ض����������ֵ��Ҫ�����������
	for (int i = 0; i < OutputImageSize.height; i++)
	{
		for (int j = 0; j < OutputImageSize.width; j++)
		{
			//������ͼ�������Ӧ��ԭͼ������
			x = j*WidthScale;
			y = i*HeightScale;
			//������ڽ����ĸ����ص������
			x1 = floor(x);
			x2 = ceil(x);
			y1 = floor(y);
			y2 = ceil(y);
			//������趨x1��x2��y1��y2
			//��x1��x2��y1��y2��Ϊ����ʱ��ֱ�ӽ�ԭͼ������ֵ������ͼ��Ķ�Ӧ����
			if (x1 == x2&&y1 == y2)
			{
				OutputPtr[i*OutputImageSize.width + j] = InputPtr[x1 + y1*InputImageSize.width];

			}
			else
			{
				//���x1λ��ԭͼ�����ұ�һ��ʱ��Ҫ��ֹ������ȷ�Χ
				if (x1 == (InputImageSize.width - 1))
				{
					x2 = x1 - 1;
				}
				//���y1λ��ԭͼ�����ұ�һ��ʱ��Ҫ��ֹ�����߶ȷ�Χ
				else if (y1 == (InputImageSize.height - 1))
				{
					y2 = y1 - 1;
				}
				//��yΪ����ʱ
				else if (x1 != x2&&y1 == y2)
				{
					y2++;
				}
				//��xΪ����
				else if(x1==x2&&y1!=y2)
				{
					x2++;
				}
				//����˫���Բ�ֵ��ʽ������ͼ�������ص�����ֵ
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
	//������ͼ������ؾ���
	return OutputImage;
}

Mat DigitalImageProcessing::Quantization(Mat InputImage, int level)
{
	//����Ҷȷ�Χ
	int LevelWidth = 256 / level;
	Mat OutputImage(InputImage.rows, InputImage.cols, CV_8UC1);
	uchar *InputPtr = InputImage.data;
	uchar *OutputPtr = OutputImage.data;
	//����ԭͼ���е��������أ���������ֵ���ڷ�Χ�趨�µĻҶ�ֵ
	//���µĻҶ�ֵ������ͼ���Ӧ������
	for (int i = 0; i < InputImage.rows; i++)
	{
		for (int j = 0; j < InputImage.cols; j++)
		{
			OutputPtr[i*OutputImage.cols + j] = ((InputPtr[i*InputImage.cols + j]) / LevelWidth+1)*LevelWidth;
		}
	}
	//������ͼ������ؾ���
	return OutputImage;
}

Mat DigitalImageProcessing::EqualizeHist(Mat InputImage)
{
	int InputImageHist[256] = { 0 };
	double L = 255;
	double coefficient = L / (InputImage.cols*InputImage.rows);//��L-1��/MN
	int SumTable[256] = { 0 };//����ÿ������ֵ����ɢ�ۻ����ʷֲ���ֵ
	Mat OutputImage(InputImage.rows, InputImage.cols, CV_8UC1);
	uchar* InputPtr = InputImage.data;
	uchar* OutputPtr = OutputImage.data;
	//ͳ�Ƹ����Ҷ�ֵ������ֵ����
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
	//�߽粹����
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

	//�������
	for (int i = row - 1; i <= (InputImage.rows + row - 1); i++)
	{
		for (int j = col - 1; j <= (InputImage.cols + col - 1); j++)
		{
			cout << i << " " << j << endl;
			int CentralPos=(row+1)/2-1;//�˲�����������ĵ�����
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

	//�Ծ������֮���õĽ��������вü�
	for (int i = row - 1; i < (InputImage.rows + row - 1); i++)
	{
		for (int j = col - 1; j < (InputImage.cols + col - 1); j++)
		{
			 OutputPtr[(i - row - 1)*InputImage.cols + (j - col - 1)]= TempPtr[i*TempImageCols + j];
		}
	}
	return OutputImage;
}

