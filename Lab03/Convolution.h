#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
#include <string.h>

using namespace std;
using namespace cv;
class Convolution
{
	Mat _kernel1;// lưu giá trị của ma trận kernel dùng để tích chập
	Mat _kernel2;// lưu giá trị của ma trận kernel dùng để tích chập
	int _kernelWidth;//chiều rộng kernel
	int _kernelHeight;//chiều dài kernel
public:
	//trả về kernel 
	vector<float> GetKernel() {
		return _kernel1;
	}
	//set kernel, chiều rộng, chiều dài kernel phải là số lẻ
	void SetKernel(Mat kernel, int kWidth, int kHeight) {
		this->_kernel1 = kernel;
		if (kWidth % 2 == 0) {
			this->_kernelWidth = kWidth - 1;
		}
		else this->_kernelWidth = kWidth;
		if (kWidth % 2 == 0) {
			this->_kernelHeight = kHeight - 1;
		}
		else this->_kernelHeight = kHeight;
	}
	void SetKernel(Mat kernel1, Mat kernel2, int kWidth, int kHeight) {
		this->_kernel1 = kernel1;  
		this->_kernel2 = kernel2;  
		if (kWidth % 2 == 0) {
			this->_kernelWidth = kWidth - 1;
		}
		else this->_kernelWidth = kWidth;
		if (kWidth % 2 == 0) {
			this->_kernelHeight = kHeight - 1;
		}
		else this->_kernelHeight = kHeight;
	}
	//Zero padding
	Mat padding(Mat sourceImage, int kWidth, int kHeight) {
		Mat source;
		sourceImage.convertTo(source, CV_64FC1);
		int paddingRows, paddingCols;
		paddingRows = (int)((kHeight - 1) / 2);
		paddingCols = (int)((kWidth - 1) / 2);
		Mat paddingImage(Size(source.cols + 2 * paddingCols, source.rows + 2 * paddingRows), CV_64FC1, Scalar(0));
		source.copyTo(paddingImage(Rect(paddingCols, paddingRows, source.cols, source.rows)));
		return paddingImage;
	}
	/*
	Hàm tính convolution của 1 ảnh xám với kernel được xác định trước
	sourceImage: ảnh input
	destinationImage: ảnh output
	Hàm trả về
	0: nếu tính thành công
	1: nếu tính thất bại (không đọc được ảnh input,...)
	*/
	int DoConvolution(const Mat& sourceImage, Mat& destinationImage, bool isPartialKernel = false, bool isMedian = false) {
		if (!sourceImage.data)
			return 1;
		
		if (isPartialKernel) {
			Mat paddingImage, kernel;
			paddingImage = padding(sourceImage, _kernelWidth, _kernelHeight);

			Mat output1 = Mat::zeros(sourceImage.size(), CV_64FC1);
			Mat output2 = Mat::zeros(sourceImage.size(), CV_64FC1);
			Mat output = Mat::zeros(sourceImage.size(), CV_64FC1);

			for (int i = 0; i < sourceImage.rows; i++) {
				for (int j = 0; j < sourceImage.cols; j++) {
					output1.at<double>(i, j) = sum(_kernel1.mul(paddingImage(Rect(j, i, _kernelWidth, _kernelHeight)))).val[0];
					output2.at<double>(i, j) = sum(_kernel2.mul(paddingImage(Rect(j, i, _kernelWidth, _kernelHeight)))).val[0];
					output.at<double>(i, j) = sqrt((output1.at<double>(i, j)) * (output1.at<double>(i, j)) + (output2.at<double>(i, j)) * (output2.at<double>(i, j)));

				}
			}
			output.convertTo(destinationImage, CV_8UC1);
		}
		else if (isMedian) {
			vector<double> temp;
			double median;
			Mat paddingImage, kernel;
			paddingImage = padding(sourceImage, _kernelWidth, _kernelHeight);

			Mat output = Mat::zeros(sourceImage.size(), CV_64FC1);

			for (int i = 0; i < sourceImage.rows; i++) {
				for (int j = 0; j < sourceImage.cols; j++) {
					temp.clear();
					for (int a = j; a < j + _kernelWidth ; a++) {
						for (int b = i; b < i + _kernelHeight; b++) {
							temp.push_back(paddingImage.at<double>(b,a));
						}
					}
					sort(temp.begin(),temp.end());
					median = temp[temp.size() / 2];
					output.at<double>(i, j) = median;

				}
			}
			output.convertTo(destinationImage, CV_8UC1);
		}
		else {

			Mat paddingImage, kernel;
			paddingImage = padding(sourceImage, _kernelWidth, _kernelHeight);

			Mat output = Mat::zeros(sourceImage.size(), CV_64FC1);

			for (int i = 0; i < sourceImage.rows; i++) {
				for (int j = 0; j < sourceImage.cols; j++) {

					output.at<double>(i, j) = sum(_kernel1.mul(paddingImage(Rect(j, i, _kernelWidth, _kernelHeight)))).val[0];
				}
			}
			output.convertTo(destinationImage, CV_8UC1);
		}

	}
	
	Convolution() {
		this->_kernelHeight = 0;
		this->_kernelWidth = 0;
		_kernel1.create(_kernelHeight, _kernelWidth, CV_64FC1);
		_kernel2.create(_kernelHeight, _kernelWidth, CV_64FC1);
		
	}
	~Convolution() {}
};

