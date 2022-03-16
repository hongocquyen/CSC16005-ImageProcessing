#pragma once
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

class ColorTransformer
{
public:
	/*
	Hàm nhận vào một ảnh, thay đổi độ sáng của ảnh này và lưu kết quả vào ảnh mới
	Tham so:
		sourceImage	: ảnh ban đầu
		destinationImage: ảnh kết quả
		b	: giá trị số nguyên dùng để thay đổi độ sáng của ảnh
	Hàm trả về:
		1: Nếu thành công thì trả về ảnh kết quả (ảnh gốc vẫn giữ nguyên giá trị)
		0: Nếu không tạo được ảnh kết quả hoặc ảnh input không tồn tại
	*/
	int ChangeBrighness(const Mat& sourceImage, Mat& destinationImage, short b) {
		if (sourceImage.data == NULL)
			return 0;
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
				short B = (short)psrcRow[0];
				short G = (short)psrcRow[1];
				short R = (short)psrcRow[2];
				B += b;
				G += b;
				R += b;
				if (B < 0)
					B = 0;
				else if (B > 255)
					B = 255;
				if (G < 0)
					G = 0;
				else if (G > 255)
					G = 255;
				if (R < 0)
					R = 0;
				else if (R > 255)
					R = 255;
				pdstRow[0] = (uchar)B;
				pdstRow[1] = (uchar)G;
				pdstRow[2] = (uchar)R;
			}
		}

		return 1;
	}

	/*
	Hàm nhận vào một ảnh, thay đổi độ tương phản của ảnh này và lưu kết quả vào ảnh mới
	Tham so :
		sourceImage : ảnh ban đầu
		destinationImage : ảnh kết quả
		c	: giá trị số thực dùng để thay đổi độ tương phản của ảnh
	Hàm trả về:
		1: Nếu thành công thì trả về ảnh kết quả (ảnh gốc vẫn giữ nguyên giá trị)
		0: Nếu không tạo được ảnh kết quả hoặc ảnh input không tồn tại
	*/
	int ChangeContrast(const Mat& sourceImage, Mat& destinationImage, float c) {
		if (sourceImage.data == NULL)
			return 0;
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
				float B = (float)psrcRow[0];
				float G = (float)psrcRow[1];
				float R = (float)psrcRow[2];
				B *= c;
				G *= c;
				R *= c;
				if (B < 0)
					B = 0;
				else if (B > 255)
					B = 255;
				if (G < 0)
					G = 0;
				else if (G > 255)
					G = 255;
				if (R < 0)
					R = 0;
				else if (R > 255)
					R = 255;
				pdstRow[0] = (uchar)B;
				pdstRow[1] = (uchar)G;
				pdstRow[2] = (uchar)R;
			}
		}

		return 1;
	}


	/*
	Hàm tính lược đồ màu tổng quát cho ảnh bất kỳ
	Tham so :
		sourceImage : ảnh ban đầu
		histMatrix : ma trận histogram (nChannels x 256), mỗi dòng là 1 mảng 256 phần tử histogram của từng kênh màu
	Hàm trả về:
		1: Nếu thành công thì trả về matrix kết quả (ảnh gốc vẫn giữ nguyên giá trị)
		0: Nếu không tính được histogram hoặc ảnh input không tồn tại
	*/
	int CalcHistogram(const Mat& sourceImage, Mat& histMatrix) {
		if (sourceImage.data == NULL)
			return 0;
		int width = sourceImage.cols, height = sourceImage.rows;
		int srcChannels = sourceImage.step[1];
		int srcwidthStep = sourceImage.step[0];


		histMatrix = Mat::zeros(srcChannels, 256,CV_8UC1);



		//int dstChannels = histMatrix.step[1];
		int dstwidthStep = histMatrix.step[0];
		uchar* psrcData = (uchar*)sourceImage.data;
		uchar* pdstData = (uchar*)histMatrix.data;
		for (int y = 0; y < height; y++, psrcData += srcwidthStep) {
			uchar* psrcRow = psrcData;
			//uchar* pdstRow = pdstData;
			//pdstRow += dstChannels
			for (int x = 0; x < width; x++, psrcRow += srcChannels) {
				//pdstDate +
				/*uchar valueB = psrcRow[0];
				uchar valueG = psrcRow[1];
				uchar valueR = psrcRow[2];*/


				//B G R
				for (int i = 0; i < 3; i++) {
					pdstData += (psrcRow[i] + dstwidthStep * (uchar)i);
					pdstData[0]++;
					pdstData = (uchar*)histMatrix.data;
				}
			}
		}
		return 1;
	}

	/*
	Hàm cân bằng sáng cho ảnh bất kỳ
	Tham so :
		srcImage : ảnh dùng ban đầu
		destinationImage : ảnh sau khi được cân bằng sáng
	Hàm trả về:
		1: Nếu thành công thì trả về ảnh kết quả (ảnh gốc vẫn giữ nguyên giá trị)
		0: Nếu không tạo được ảnh kết quả hoặc ảnh input không tồn tại
	*/
	int HistogramEqualization(const Mat& sourceImage, Mat& destinationImage) {
		if (sourceImage.data == NULL)
			return 0;
		return 1;
	}



	/*
Hàm cân bằng lược đồ màu tổng quát cho ảnh bất kỳ
Tham so :
	histMatrix : ma trận histogram đã tính được
	histImage : ảnh histogram được vẽ
Hàm trả về:
	1: Nếu thành công vẽ được histogram
	0: Nếu không vẽ được histogram
*/
	int DrawHistogram(const Mat& histMatrix, Mat& histImage) {
		if (histMatrix.data == NULL)
			return 0;

		int nChannel = histMatrix.step[1];
		int widthStep = histMatrix.step[0];
		int thickness = 2;
		int delta = 10;
		histImage = Mat(300, 257*3*thickness +delta*3*thickness, CV_8UC1,Scalar(0,0,0));

		
		uchar* pData = (uchar*)histMatrix.data;
		
		for (int i = 0; i < 3; i++,pData+= widthStep) {
			uchar* pRow = pData;
			for (int j = (256+delta)* i; j <= (256+delta) *(i+1); j++)
			{
				if (j > (256 + delta) * i && j <= (256 + delta) * i + 256)
				{
					rectangle(histImage, Point(thickness * (j), histImage.rows - pRow[0]),
						Point(thickness * (j + 1), histImage.rows), Scalar(255, 255, 255));
					pRow += nChannel;
				}
				//Draw space between (delta)
				else {
					rectangle(histImage, Point(thickness * j, histImage.rows),
						Point(thickness * (j + 1), histImage.rows), Scalar(255, 0, 255));
				}
			}
		}

		return 1;
	}

	ColorTransformer() {}
	~ColorTransformer() {}
};

