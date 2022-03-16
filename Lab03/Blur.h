#pragma once
#include "Convolution.h"
class Blur
{
public:

	Mat _kernel(int _kernelWidth, int _kernelHeight, int kernelType) {

		switch (kernelType) {
			//Average
		case 1:
		{
			Mat kernel(_kernelHeight, _kernelWidth, CV_64FC1, Scalar(1.0 / (_kernelWidth * _kernelHeight)));
			return kernel;

		}
			//Median
		case 2:
		{
			Mat kernel(_kernelWidth, _kernelHeight, CV_64FC1, Scalar(0));

			
			return kernel;
		}
			//Gaussian
		case 3: {
			int paddingRows = (_kernelHeight - 1) / 2;
			int paddingCols = (_kernelWidth - 1) / 2;
			Mat kernel(_kernelHeight, _kernelWidth, CV_64FC1);
			int o = 3;
			for (int i = -paddingRows; i <= paddingRows; i++) {
				for (int j = -paddingCols; j <= paddingCols; j++) {
					kernel.at<double>(i + paddingRows, j + paddingCols) = (1.0 / (sqrt(2 * 3.1415) * o)) * exp(-(i * i + j * j) / (2.0 * o * o));
				}
			}
			kernel = kernel / sum(kernel);

			return kernel;
		}
		}
	}
	/*
	Hàm làm trơn ảnh xám
	sourceImage: ảnh input
	destinationImage: ảnh output
	method: phương pháp detect
	kWidth, kHeight: kích thước kernel
	0: Trung bình
	1: Trung vị
	2: Gaussian
	Hàm trả về
	0: nếu detect thành công
	1: nếu detect thất bại (không đọc được ảnh input,...)
	*/
	int BlurImage(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight, int method) {
		if (!sourceImage.data)
			return 1;

		switch (method) {
			//Average
		case 0:
		{
			Convolution c;
			c.SetKernel(_kernel(3, 3, 1), 3, 3);
			c.DoConvolution(sourceImage, destinationImage);

			break;
		}
		case 1:
		{
			Convolution c;
			c.SetKernel(_kernel(3, 3, 2), 3, 3);
			c.DoConvolution(sourceImage, destinationImage, false, true);

			break;
		}
		case 2:
		{
			Convolution c;
			c.SetKernel(_kernel(3, 3, 3), 3, 3);
			c.DoConvolution(sourceImage, destinationImage);

			break;
		}
		}
		return 0;
	}
	Blur() {}
	~Blur() {} 
};

