#pragma once
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

#define PI 3.1415926535

/*
 Lớp base dùng để nội suy màu của 1 pixel
*/
class PixelInterpolate
{
public:
	/*
	Hàm tính giá trị màu của ảnh kết quả từ nội suy màu trong ảnh gốc
	Tham số
		- (tx,ty): tọa độ thực của ảnh gốc sau khi thực hiện phép biến đổi affine
		- pSrc: con trỏ ảnh gốc
		- srcWidthStep: widthstep của ảnh gốc
		- nChannels: số kênh màu của ảnh gốc
	Trả về
		- Giá trị màu được nội suy
	*/
	virtual void Interpolate(
		float tx, float ty, uchar* pSrc, int srcWidthStep, int nChannels,uchar* pDstRow) = 0;
	PixelInterpolate() {};
	~PixelInterpolate() {};
};

/*
Lớp nội suy màu theo phương pháp song tuyến tính
*/
class BilinearInterpolate : public PixelInterpolate
{
public:
	void Interpolate(float tx, float ty, uchar* pSrc, int srcWidthStep, int nChannels,uchar * pDstRow) {
	
		int x = floor(tx), y = floor(ty);
		float a = fabs(tx - x), b = fabs(ty - y); // [0,1]

		uchar* pSrc00 = pSrc + (y * srcWidthStep + x * nChannels); // z00
		uchar* pSrc10 = pSrc + (y * srcWidthStep + (x + 1) * nChannels); // z10
		uchar* pSrc01 = pSrc + ((y + 1) * srcWidthStep + x * nChannels); // z01
		uchar* pSrc11 = pSrc + ((y + 1) * srcWidthStep + (x + 1) * nChannels); // z11
		for (int i = 0; i < nChannels; i++)
			pDstRow[i] = saturate_cast<uchar>((1 - a) * (1 - b) * pSrc00[i] + a * (1 - b) * pSrc10[i] + b * (1 - a) * pSrc01[i] + a * b * pSrc11[i]); //pDst[i] in [0,255]
		
	};
	BilinearInterpolate() {};
	~BilinearInterpolate() {};
};

/*
Lớp nội suy màu theo phương pháp láng giềng gần nhất
*/
class NearestNeighborInterpolate : public PixelInterpolate
{
public:
	void Interpolate(float tx, float ty, uchar* pSrc, int srcWidthStep, int nChannels,uchar* pDstRow) {
		int x = round(tx), y = round(ty);
		uchar* pSrcRow = pSrc + (x * nChannels + y * srcWidthStep);

		for (int i = 0; i < nChannels; i++)
		{
			pDstRow[i] = pSrcRow[i];
		}
		
	}
	NearestNeighborInterpolate() {};
	~NearestNeighborInterpolate() {};
};

/*
Lớp biểu diễn pháp biến đổi affine
*/
class AffineTransform
{
	Mat _matrixTransform;//ma trận 3x3 biểu diễn phép biến đổi affine
public:
	// xây dựng matrix transform cho phép tịnh tiến theo vector (dx,dy)
	void Translate(float dx, float dy) {
		Mat t;
		t.create(3, 3, CV_32FC1);

		int width = t.cols;
		int height = t.rows;

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				if (x == y) {
					t.at<float>(x, y) = 1;
				}
				else t.at<float>(x, y) = 0;
			}
		}
		//t.at<float>(0, 0) = -1;
		t.at<float>(0, 2) = dx;
		t.at<float>(1, 2) = dy;

		_matrixTransform = _matrixTransform * t;
	}
	//xây dựng matrix transform cho phép xoay 1 góc angle
	void Rotate(float angle) {
		Mat t;
		t.create(3, 3, CV_32FC1);

		int width = t.cols;
		int height = t.rows;

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				if (x == y) {
					t.at<float>(x, y) = 1;
				}
				else t.at<float>(x, y) = 0;
			}
		}

		angle = angle* PI / 180;

		t.at<float>(0, 0) = cosf(angle);
		t.at<float>(0, 1) = -sinf(angle);
		t.at<float>(1, 0) = sinf(angle);
		t.at<float>(1, 1) = cosf(angle);

		_matrixTransform = _matrixTransform * t;
	}
	//xây dựng matrix transform cho phép tỉ lệ theo hệ số 		
	void Scale(float sx, float sy) {
		Mat t;
		t.create(3, 3, CV_32FC1);

		int width = t.cols;
		int height = t.rows;

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				if (x == y) {
					t.at<float>(x, y) = 1;
				}
				else t.at<float>(x, y) = 0;
			}
		}

		t.at<float>(0, 0) = sx;
		t.at<float>(1, 1) = sy;

		_matrixTransform = _matrixTransform * t;
	}
	//transform 1 điểm (x,y) theo matrix transform đã có
	void TransformPoint(float& x, float& y) {
		float M[3] = { x,y,1 }; //Ma trận điểm ảnh nguồn
		float Mc[3] = { 0, 0, 0 }; //Ma trận điểm ảnh đích

		for (int i = 0; i < _matrixTransform.rows; i++) {
			for (int j = 0; j < _matrixTransform.cols; j++) {
				Mc[i] += _matrixTransform.at<float>(i, j) * M[j]; //Nhân 2 ma trận 3x3 * 3x1
			}
		}
		
		//Gán lại x',y'
		x = Mc[1];
		y = Mc[0];
	}
	//Hàm cập nhật matrixTransform
	void UpdateMatrix(Mat t) {
		_matrixTransform = _matrixTransform*t;
	}
	AffineTransform() {
		_matrixTransform.create(3, 3, CV_32FC1);
		int width = _matrixTransform.cols;
		int height = _matrixTransform.rows;

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				if (x == y) {
					_matrixTransform.at<float>(x, y) = 1;
				}
				else _matrixTransform.at<float>(x, y) = 0;
			}
		}
	}
	~AffineTransform() {}
};

/*
Lớp thực hiện phép biến đổi hình học trên ảnh
*/

class GeometricTransformer
{
public:
	/*
	Hàm biến đổi ảnh theo 1 phép biến đổi affine đã có
	Tham số
	 - beforeImage: ảnh gốc trước khi transform
	 - afterImage: ảnh sau khi thực hiện phép biến đổi affine
	 - transformer: phép biến đổi affine
	 - interpolator: biến chỉ định phương pháp nội suy màu
	Trả về:
	 - 0: Nếu ảnh input ko tồn tại hay ko thực hiện được phép biến đổi
	 - 1: Nếu biến đổi thành công
	*/
	int Transform(const Mat& beforeImage,Mat& afterImage,AffineTransform* transformer,PixelInterpolate* interpolator) {
		if (beforeImage.data == NULL)
			return 0;

		int bwidth = beforeImage.cols, bheight = beforeImage.rows;
		int awidth = afterImage.cols, aheight = afterImage.rows;

		int bChannels = beforeImage.step[1];
		int bwidthStep = beforeImage.step[0];

		int aChannels = afterImage.step[1];
		int awidthStep = afterImage.step[0];

		uchar* bpData = (uchar*)beforeImage.data;
		uchar* apData = (uchar*)afterImage.data;

		float x, y;
		for (int i = 0; i < aheight; i++, apData += awidthStep) {
			uchar* apRow = apData;
			for (int j = 0; j < awidth; j++, apRow += aChannels) {
				x = i;
				y = j;

				transformer->TransformPoint(x, y);
				if (0 <= round(x) && round(x) < bwidth-1 && 0 <= round(y) && round(y) < bheight-1)
					interpolator->Interpolate(x, y, bpData, bwidthStep, bChannels,apRow);
			}
		}

		return 1;
	}

	/*
	Hàm xoay bảo toàn nội dung ảnh theo góc xoay cho trước
	Tham số
	- srcImage: ảnh input
	- dstImage: ảnh sau khi thực hiện phép xoay
	- angle: góc xoay (đơn vị: độ)
	- interpolator: biến chỉ định phương pháp nội suy màu
	Trả về:
	 - 0: Nếu ảnh input ko tồn tại hay ko thực hiện được phép biến đổi
	 - 1: Nếu biến đổi thành công
	*/
	int RotateKeepImage(const Mat& srcImage, Mat& dstImage, float angle, PixelInterpolate* interpolator) {
		if (srcImage.data == NULL)
			return 0;

		AffineTransform* affine = new AffineTransform();
		float rangle = angle * PI / 180;

		int srcWidth = srcImage.cols, srcHeight = srcImage.rows;
		int dstWidth = srcWidth * fabs(cosf(rangle)) + srcHeight * fabs(sinf(rangle));
		int dstHeight = srcWidth * fabs(sinf(rangle)) + srcHeight * fabs(cosf(rangle));


		float srcX0 = srcHeight / 2;
		float srcY0 = srcWidth / 2;
		float dstX0 = dstHeight / 2;
		float dstY0 = dstWidth / 2;

		int srcChannels = srcImage.step[1];
		int srcWidthStep = srcImage.step[0];

		dstImage.create(dstHeight, dstWidth, srcImage.type());
		//affineTf->Rotate(-angle);
		affine->Translate(srcX0, srcY0);
		affine->Rotate(-angle);
		affine->Translate(-dstX0, -dstY0);

		Transform(srcImage, dstImage, affine, interpolator);

		return 1;
	}

	/*
	Hàm xoay không bảo toàn nội dung ảnh theo góc xoay cho trước
	Tham số
	- srcImage: ảnh input
	- dstImage: ảnh sau khi thực hiện phép xoay
	- angle: góc xoay (đơn vị: độ)
	- interpolator: biến chỉ định phương pháp nội suy màu
	Trả về:
	 - 0: Nếu ảnh input ko tồn tại hay ko thực hiện được phép biến đổi
	 - 1: Nếu biến đổi thành công
	*/
	int RotateUnkeepImage(const Mat& srcImage, Mat& dstImage, float angle, PixelInterpolate* interpolator) {
		if (srcImage.data == NULL)
			return 0;

		AffineTransform* affineTf = new AffineTransform();
		float rangle = angle * PI / 180;

		int srcWidth = srcImage.cols, srcHeight = srcImage.rows;
		int dstWidth = srcWidth;
		int dstHeight = srcHeight;


		float srcX0 = srcHeight / 2;
		float srcY0 = srcWidth / 2;
		float dstX0 = dstHeight / 2;
		float dstY0 = dstWidth / 2;

		int srcChannels = srcImage.step[1];
		int srcWidthStep = srcImage.step[0];

		dstImage.create(dstHeight, dstWidth, srcImage.type());
		//affineTf->Rotate(-angle);
		affineTf->Translate(srcX0, srcY0);
		affineTf->Rotate(-angle);
		affineTf->Translate(-dstX0, -dstY0);

		Transform(srcImage, dstImage, affineTf, interpolator);

		return 1;
	}

	/*
	Hàm phóng to, thu nhỏ ảnh theo tỉ lệ cho trước
	Tham số
	- srcImage: ảnh input
	- dstImage: ảnh sau khi thực hiện phép xoay
	- sx, sy: tỉ lệ phóng to, thu nhỏ ảnh
	- interpolator: biến chỉ định phương pháp nội suy màu
	Trả về:
	 - 0: Nếu ảnh input ko tồn tại hay ko thực hiện được phép biến đổi
	 - 1: Nếu biến đổi thành công
	*/
	int Scale(const Mat& srcImage, Mat& dstImage, float sx, float sy, PixelInterpolate* interpolator) {
		if (srcImage.data == NULL)
			return 0;


		AffineTransform* affine = new AffineTransform();
		dstImage.create(round(srcImage.rows*sy), round(srcImage.cols*sx), srcImage.type());
		affine->Scale(1.0 / sy, 1.0 / sx);
		return Transform(srcImage,dstImage,affine,interpolator);
	}


	/*
	Hàm thay đổi kích thước ảnh
	Tham số
	- srcImage: ảnh input
	- dstImage: ảnh sau khi thay đổi kích thước
	- newWidth, newHeight: kích thước mới
	- interpolator: biến chỉ định phương pháp nội suy màu
	Trả về:
	 - 0: Nếu ảnh input ko tồn tại hay ko thực hiện được phép biến đổi
	 - 1: Nếu biến đổi thành công
	*/
	int Resize(const Mat& srcImage, Mat& dstImage, int newWidth, int newHeight, PixelInterpolate* interpolator) {
		if (srcImage.data == NULL)
			return 0;


		AffineTransform* affine = new AffineTransform();

		dstImage.create(newHeight, newWidth, srcImage.type());

		float sx = newWidth*1.0/ srcImage.cols;
		float sy = newHeight*1.0/ srcImage.rows;
		affine->Scale(1.0 / sy, 1.0 / sx);

		
		return Transform(srcImage, dstImage, affine, interpolator);
	}

	/*
	Hàm lấy đối xứng ảnh
	Tham số
	- srcImage: ảnh input
	- dstImage: ảnh sau khi lấy đối xứng
	- direction = 1 nếu lấy đối xứng theo trục ngang và direction = 0 nếu lấy đối xứng theo trục đứng
	- interpolator: biến chỉ định phương pháp nội suy màu
	Trả về:
	 - 0: Nếu ảnh input ko tồn tại hay ko thực hiện được phép biến đổi
	 - 1: Nếu biến đổi thành công
	*/
	int Flip(const Mat& srcImage, Mat& dstImage, bool direction, PixelInterpolate* interpolator) {
		if (srcImage.data == NULL)
			return 0;


		AffineTransform* affine = new AffineTransform();

		dstImage.create(srcImage.rows, srcImage.cols, srcImage.type());

		if (direction == 1) {
			
			Mat t;
			t.create(3, 3, CV_32FC1);

			int width = t.cols;
			int height = t.rows;

			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					if (x == y) {
						t.at<float>(x, y) = 1;
					}
					else t.at<float>(x, y) = 0;
				}
			}
			t.at<float>(0, 0) = -1;
			t.at<float>(0, 2) = srcImage.rows - 1;
			t.at<float>(1, 2) = 0;

			affine->UpdateMatrix(t);
		}
		else if (direction == 0) {
			Mat t;
			t.create(3, 3, CV_32FC1);

			int width = t.cols;
			int height = t.rows;

			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					if (x == y) {
						t.at<float>(x, y) = 1;
					}
					else t.at<float>(x, y) = 0;
				}
			}
			t.at<float>(1, 1) = -1;
			t.at<float>(0, 2) = 0;
			t.at<float>(1, 2) = srcImage.cols - 1;

			affine->UpdateMatrix(t);
		}
		return Transform(srcImage, dstImage, affine, interpolator);
	}

	GeometricTransformer() {};
	~GeometricTransformer() {};
};

