#include "GeometricTransformer.h"
#include <string.h>

using namespace std;

int main(int argc, char* argv[])
{
	Mat srcImage, dstImage;
	
	GeometricTransformer GT;
	PixelInterpolate* pixInterpolate;

	int result = 0; 


	if (string(argv[1]) == "-zoom") {
		srcImage = imread(string(argv[5]));
		float sx = stof(argv[3]); 
		float sy = stof(argv[4]); 
		if (string(argv[2]) == "-bl")
		{
			pixInterpolate = new BilinearInterpolate();
			result = GT.Scale(srcImage, dstImage, sx, sy, pixInterpolate);
			
		}
		else if (string(argv[2]) == "-nn")
		{
			pixInterpolate = new NearestNeighborInterpolate();
			result = GT.Scale(srcImage, dstImage, sx, sy, pixInterpolate);
		}
		if(result==1)
			imwrite(string(argv[6]), dstImage);
		return result;
	}
	else if (string(argv[1]) == "-resize") {
		srcImage = imread(string(argv[5]));
		int newWidth = stoi(argv[3]); 
		int newHeight = stoi(argv[4]); 
		if (string(argv[2]) == "-bl")
		{
			pixInterpolate = new BilinearInterpolate();
			result = GT.Resize(srcImage, dstImage, newWidth, newHeight, pixInterpolate);
		}
		else if (string(argv[2]) == "-nn")
		{
			pixInterpolate = new NearestNeighborInterpolate();
			result = GT.Resize(srcImage, dstImage, newWidth, newHeight, pixInterpolate);
		}
		if (result == 1)
			imwrite(string(argv[6]), dstImage);
		return result;
	}
	else if (string(argv[1]) == "-rotK") {
		srcImage = imread(string(argv[4]));
		float angle = stof(argv[3]); 
		if (string(argv[2]) == "-bl")
		{
			pixInterpolate = new BilinearInterpolate();
			result = GT.RotateUnkeepImage(srcImage, dstImage, angle, pixInterpolate);
		}
		else if (string(argv[2]) == "-nn")
		{
			pixInterpolate = new NearestNeighborInterpolate();
			result = GT.RotateUnkeepImage(srcImage, dstImage, angle, pixInterpolate);
		}
		if (result == 1)
			imwrite(string(argv[5]), dstImage);
		return result;
	}
	else if (string(argv[1]) == "-rotP") {
		srcImage = imread(string(argv[4]));
		float angle = stof(argv[3]);
		if (string(argv[2]) == "-bl")
		{
			pixInterpolate = new BilinearInterpolate();
			result = GT.RotateKeepImage(srcImage, dstImage, angle, pixInterpolate);
		}
		else if (string(argv[2]) == "-nn")
		{
			pixInterpolate = new NearestNeighborInterpolate();
			result = GT.RotateKeepImage(srcImage, dstImage, angle, pixInterpolate);
		}
		if (result == 1)
			imwrite(string(argv[5]), dstImage);
		return result;
	}
	else if (string(argv[1]) == "-flipV") {
		srcImage = imread(string(argv[3]));
		if (string(argv[2]) == "-bl")
		{
			pixInterpolate = new BilinearInterpolate();
			result = GT.Flip(srcImage, dstImage, 0 , pixInterpolate);
		}
		else if (string(argv[2]) == "-nn")
		{
			pixInterpolate = new NearestNeighborInterpolate();
			result = GT.Flip(srcImage, dstImage, 0, pixInterpolate);
		}
		if (result==1)
			imwrite(string(argv[4]), dstImage);
		return result;
	}
	else if (string(argv[1]) == "-flipH") {
		srcImage = imread(string(argv[3]));
		if (string(argv[2]) == "-bl")
		{
			pixInterpolate = new BilinearInterpolate();
			result = GT.Flip(srcImage, dstImage, 1, pixInterpolate);
		}
		else if (string(argv[2]) == "-nn")
		{
			pixInterpolate = new NearestNeighborInterpolate();
			result = GT.Flip(srcImage, dstImage, 1, pixInterpolate);
		}
		if (result == 1)
			imwrite(string(argv[4]), dstImage);
		return result;
	}
	else
		cout<<"Khong dung dinh dang command line"<<endl;

	waitKey(0);
	return 0;
}