#include "Converter.h"
#include "ColorTransformer.h"

int main(int argc, char* argv[]) {

	if (argc == 4) {
		Converter con;
		Mat srcImage = imread(string(argv[2]));

		Mat dstImage;
		int type = 1;

		ColorTransformer ctrans;
		Mat histMatrix;
		Mat histImage;
		int rs = 0;
		

		if (string(argv[1]) == "-rgb2gray")
		{
			con.Convert(srcImage, dstImage, 0);
			imwrite(argv[3], dstImage);
		}
		else if (string(argv[1]) == "-rgb2hsv")
		{
			con.Convert(srcImage, dstImage, 1);
			imwrite(argv[3], dstImage);
		}
		else if (string(argv[1]) == "-drawhist")
		{
			ctrans.CalcHistogram(srcImage, histMatrix);
			ctrans.DrawHistogram(histMatrix, histImage);

			imwrite(argv[3], histImage);
		}
		
		waitKey(0);
		return 0;
	}

	else if (argc == 5) {
		Mat srcImage = imread(string(argv[3]));

		Mat dstImage;

		ColorTransformer ctrans;


		if (string(argv[1]) == "-bright") {
			short b;
			stringstream ss(argv[2]);
			ss >> b;
			ctrans.ChangeBrighness(srcImage, dstImage, b);

			imwrite(argv[4], dstImage);
		}
		else if (string(argv[1]) == "-contrast") {
			ctrans.ChangeContrast(srcImage, dstImage, stof(argv[2]));

			imwrite(argv[4], dstImage);
		}
		waitKey(0);
		return 0;
	}

	else {
		cout << "Command line khong dung dinh dang" << endl;
		cout<<"Dinh dang: "<<endl;
		cout << "\t<Program.exe> -rgb2gray <InputFilePath> <OutputFilePath>" << endl;
		cout << "\t<Program.exe> -rgb2hsv <InputFilePath> <OutputFilePath>" << endl;
		cout << "\t<Program.exe> -drawhist <InputFilePath> <OutputFilePath>" << endl;
		cout << "\t<Program.exe> -bright <b> <InputFilePath> <OutputFilePath>" << endl;
		cout << "\t<Program.exe> -contrast <c> <InputFilePath> <OutputFilePath>" << endl;
		return 0;
	}
}
