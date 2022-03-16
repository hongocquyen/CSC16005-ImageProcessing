#include "EdgeDetector.h"
#include "Blur.h"

int main(int argc, char* argv[])
{
	Mat srcImage, dstImage;

	EdgeDetector edge;
	Blur bl;

	int result = 0;


	if (string(argv[1]) == "-avg") {
		srcImage = imread(string(argv[4]),0);
		int kx = stoi(argv[2]);
		int ky = stoi(argv[3]);
		
		result = bl.BlurImage(srcImage, dstImage, kx, ky, 0);
		if (result == 0)
			imwrite(string(argv[5]), dstImage);
		return result;
	}
	else if (string(argv[1]) == "-med") {
		srcImage = imread(string(argv[4]),0);
		int kx = stoi(argv[2]);
		int ky = stoi(argv[3]);

		result = bl.BlurImage(srcImage, dstImage, kx, ky, 1);
		if (result == 0)
			imwrite(string(argv[5]), dstImage);
		return result;
	}
	else if (string(argv[1]) == "-gau") {
		srcImage = imread(string(argv[4]),0);
		int kx = stoi(argv[2]);
		int ky = stoi(argv[3]);

		result = bl.BlurImage(srcImage, dstImage, kx, ky, 2);
		if (result == 0)
			imwrite(string(argv[5]), dstImage);
		return result;
	}
	else if (string(argv[1]) == "-sobel") {
		srcImage = imread(string(argv[2]),0);

		result = edge.DetectEdge(srcImage, dstImage, 1);
		if (result == 0)
			imwrite(string(argv[3]), dstImage);
		return result;
	}
	else if (string(argv[1]) == "-prew") {
		srcImage = imread(string(argv[2]),0);

		result = edge.DetectEdge(srcImage, dstImage, 2);
		if (result == 0)
			imwrite(string(argv[3]), dstImage);
		return result;
	}
	else if (string(argv[1]) == "-lap") {
		srcImage = imread(string(argv[2]),0);

		result = edge.DetectEdge(srcImage, dstImage, 3);
		if (result == 0)
			imwrite(string(argv[3]), dstImage);
		return result;
	}
	else
		cout << "Khong dung dinh dang command line" << endl;

	waitKey(0);
	return 0;
}