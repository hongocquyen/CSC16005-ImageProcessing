#pragma once
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

class Converter
{
	/*
	Hàm chuyển đổi không gian màu của ảnh từ RGB sang GrayScale
	sourceImage: ảnh input
	destinationImage: ảnh output
	Hàm trả về
	0: nếu chuyển thành công
	1: nếu chuyển thất bại (không đọc được ảnh input,...)
	*/
	int RGB2GrayScale( Mat& sourceImage, Mat& destinationImage) {
		if (sourceImage.data == NULL)
			return 1;
		int width = sourceImage.cols, height = sourceImage.rows;
		int srcChannels = sourceImage.step[1];
		int srcwidthStep = sourceImage.step[0];


		destinationImage = Mat(height,width, CV_8UC1);
		int dstChannels = destinationImage.step[1];
		int dstwidthStep = destinationImage.step[0];

		uchar* psrcData = (uchar*)sourceImage.data;
		uchar* pdstData = (uchar*)destinationImage.data;
		for (int y = 0; y < height; y++, psrcData += srcwidthStep, pdstData +=dstwidthStep) {
			uchar* psrcRow = psrcData;
			uchar* pdstRow = pdstData;
			for (int x = 0; x < width; x++, psrcRow += srcChannels, pdstRow += dstChannels) {
				uchar B = psrcRow[0];
				uchar G = psrcRow[1];
				uchar R = psrcRow[2];
				uchar grayScale_value = (uchar)(0.299*R + 0.587*G+0.114*B);
				pdstRow[0] = grayScale_value;
			}
		}
			
		return 0;
	}

	/*
	Hàm chuyển đổi không gian màu của ảnh từ RGB sang HSV
	sourceImage: ảnh input
	destinationImage: ảnh output
	Hàm trả về
	0: nếu chuyển thành công
	1: nếu chuyển thất bại (không đọc được ảnh input,...)
	*/
	int RGB2HSV(const Mat& sourceImage, Mat& destinationImage) {
		if (sourceImage.data == NULL)
			return 1;
		int width = sourceImage.cols, height = sourceImage.rows;
		int srcChannels = sourceImage.step[1];
		int srcwidthStep = sourceImage.step[0];


		destinationImage = Mat(height, width, CV_8UC3);
		int dstChannels = destinationImage.step[1];
		int dstwidthStep = destinationImage.step[0];

		uchar* psrcData = (uchar*)sourceImage.data;
		uchar* pdstData = (uchar*)destinationImage.data;
		for (int y = 0; y < height; y++, psrcData += srcwidthStep, pdstData += dstwidthStep) {
			uchar* psrcRow = psrcData;
			uchar* pdstRow = pdstData;
			for (int x = 0; x < width; x++, psrcRow += srcChannels, pdstRow += dstChannels) {			
				double B = (double)psrcRow[0];
				double G = (double)psrcRow[1];
				double R = (double)psrcRow[2];
				double Bx = B / 255.0;
				double Gx = G / 255.0;
				double Rx = R / 255.0;

				double Cmax = 0, Cmin = 0;
				Cmax = Bx;
				if (Cmax < Gx) {
					Cmax = Gx;
				}
				if (Cmax < Rx)
					Cmax = Rx;

				Cmin = Bx;
				if (Cmin > Gx)
					Cmin = Gx;
				if (Cmin > Rx)
					Cmin = Rx;
				double delta = Cmax - Cmin;

				double H;
				double S;
				double V;
				if (delta == 0)
					H = 0;
				else if (Rx == Cmax) {
					H = fmod(((60 * ((Gx - Rx) / delta)) + 360), 360.0);
				}
				else if (Gx == Cmax) {
					H = fmod(((60 * ((Bx - Rx) / delta)) + 120), 360.0);
				}
				else if (Bx = Cmax) {
					H = fmod(((60 * ((Rx - Gx) / delta)) + 240), 360.0);
				}

				if (H < 0)
					H += 360;
				H = H / 2;
				
				if (Cmax == 0)
					S = 0;
				else S = delta / Cmax;
				S = 255 * S;

				V = Cmax;
				V = 255 * V;

				pdstRow[0] = (uchar)H;
				pdstRow[1] = (uchar)S;
				pdstRow[2] = (uchar)V;
			}
		}

		return 0;
	}

public:
	/*1
	Hàm chuyển đổi không gian màu của ảnh
	sourceImage: ảnh input
	destinationImage: ảnh output cùng kích thước, cùng loại với ảnh input
	type: loại chuyển đổi
		0: chuyển từ RGB sang GrayScale
		1: chuyển từ RGB sang HSV
	Hàm trả về
		0: nếu chuyển thành công
		1: nếu chuyển thất bại (không đọc được ảnh input hay type không chính xác,...)
	*/
		int Convert(Mat & sourceImage, Mat & destinationImage, int type){
			if (type == 0) {
				return RGB2GrayScale(sourceImage, destinationImage);
			}
			else if (type == 1) {
				return RGB2HSV(sourceImage,destinationImage);
			}
		}

		Converter() {};
		~Converter() {};
};

