#pragma once

#include "Convolution.h"

class EdgeDetector
{
public:
	Mat _kernel(int _kernelWidth, int _kernelHeight, int kernelType) {
		
		switch (kernelType) {
			//Sobelx
		case 0:
		{
			Mat kernel(_kernelHeight, _kernelWidth, CV_64FC1, Scalar(0));
			for (int j = -_kernelHeight / 2, y = 0; j <= _kernelHeight / 2; y++, j++) {
				for (int i = -_kernelWidth / 2, x = 0; i <= _kernelWidth / 2; x++, i++) {
					if (i == 0 && j == 0)
					{

					}
					else kernel.at<double>(y, x) = ((i) / ((i * i) + (j * j)))*2;
				}
			}
			return kernel;

			//Mat kernel(3, 3, CV_64FC1, Scalar(0));
			////Kernel ngược để thực hiện tích 2 ma trận theo chiều xuôi , kernel*f(x,y) 
			//kernel.at<double>(0, 0) = 1;
			//kernel.at<double>(1, 0) = 2;
			//kernel.at<double>(2, 0) = 1;

			//kernel.at<double>(0, 2) = -1;
			//kernel.at<double>(1, 2) = -2;
			//kernel.at<double>(2, 2) = -1;
			//return kernel;

		}
			//Sobely
		case 1:
		{
			Mat kernel(_kernelHeight, _kernelWidth, CV_64FC1, Scalar(0));
			for (int j = -_kernelHeight / 2, y = 0; j <= _kernelHeight / 2; y++, j++) {
				for (int i = -_kernelWidth / 2, x = 0; i <= _kernelWidth / 2; x++, i++) {
					if (i == 0 && j == 0)
					{

					}
					else kernel.at<double>(y, x) = (j) / ((i * i) + (j * j));
				}
			}
			return kernel;

			//Mat kernel(3, 3, CV_64FC1, Scalar(0));
			////Kernel ngược để thực hiện tích 2 ma trận theo chiều xuôi , kernel*f(x,y) 
			//kernel.at<double>(0, 0) = -1;
			//kernel.at<double>(0, 1) = -2;
			//kernel.at<double>(0, 2) = -1;

			//kernel.at<double>(2, 0) = 1;
			//kernel.at<double>(2, 1) = 2;
			//kernel.at<double>(2, 2) = 1;
			//return kernel;

		}
			//Prewittx
		case 2:
		{
			Mat kernel(_kernelWidth, _kernelHeight, CV_64FC1, Scalar(0));

			kernel.at<double>(0, 0) = 1;
			kernel.at<double>(1, 0) = 1;
			kernel.at<double>(2, 0) = 1;

			kernel.at<double>(0, 2) = -1;
			kernel.at<double>(1, 2) = -1;
			kernel.at<double>(2, 2) = -1;
			return kernel;
		}
			//Prewitty
		case 3: {
			Mat kernel(_kernelWidth, _kernelHeight, CV_64FC1, Scalar(0));

			kernel.at<double>(0, 0) = -1;
			kernel.at<double>(0, 1) = -1;
			kernel.at<double>(0, 2) = -1;

			kernel.at<double>(2, 0) = 1;
			kernel.at<double>(2, 1) = 1;
			kernel.at<double>(2, 2) = 1;

			return kernel;
		}
		case 4: {

			Mat kernel(_kernelWidth, _kernelHeight, CV_64FC1, Scalar(-1));

			kernel.at<double>(1, 1) = (_kernelWidth * _kernelHeight) - 1;

			return kernel;
		}
		}
	}

	/*
	Hàm phát hiện biên cạnh của ảnh xám
	sourceImage: ảnh input
	destinationImage: ảnh output
	method: phương pháp detect
		1: Sobel
		2: Prewitt
		3: Laplace
	Hàm trả về
		0: nếu detect thành công
		1: nếu detect thất bại (không đọc được ảnh input,...)
	*/
	int DetectEdge(const Mat& sourceImage, Mat& destinationImage, int method) {
		if (!sourceImage.data)
			return 1;
		switch (method) {
				
		case 1:
		{
			Convolution c;
			c.SetKernel(_kernel(3, 3, 0), _kernel(3, 3, 1), 3, 3);
			c.DoConvolution(sourceImage, destinationImage, true);

			break;
		}
		case 2:
		{
			Convolution c;
			c.SetKernel(_kernel(3, 3, 2), _kernel(3, 3, 3), 3, 3);
			c.DoConvolution(sourceImage, destinationImage, true);
			break;
		}
		//Laplace
		case 3:
		{
			Convolution c;
			c.SetKernel(_kernel(3, 3, 2), 3, 3);
			c.DoConvolution(sourceImage, destinationImage);
			break;
		}
		}
		return 0;
	}
	EdgeDetector() {}
	~EdgeDetector() {}
};

