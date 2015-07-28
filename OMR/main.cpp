#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <ctime>

#include "OMR.h"

using namespace cv;
using namespace std;

string findName(char * name)
{
	string res = "";
	int len = strlen(name);
	for (int i = 0; i < len; i++)
	{
		if (name[i] == '.') break;
		res = res + name[i];
	}
	return res;
}

int main(int argc, char * argv []) 
{
	srand(time(NULL));
	if (argc == 2)
	{
		// input image
		Mat image = imread(argv[1]);

		OMR OMRSystem(image);
		string fileName = findName(argv[1]);
		imwrite(fileName + "_staff_removed.jpg", OMRSystem.GetMusicSheetImage());
		imwrite(fileName + "_staff_removed_color.jpg", OMRSystem.GetMusicSheetBgrImage());
	}
	else
	{
		Mat image = imread("data/testset/5.png");
		OMR OMRSystem(image);
		namedWindow("Image1", CV_WINDOW_AUTOSIZE);
		namedWindow("Image2", CV_WINDOW_AUTOSIZE);
		imshow("Image1", OMRSystem.GetMusicSheetBgrImage());
		imshow("Image2", OMRSystem.GetMusicSheetImage());
		imwrite("data/binarized.jpg", OMRSystem.GetBinarizedImage());
		imwrite("data/result.jpg", OMRSystem.GetMusicSheetImage());
		imwrite("data/resultBgr.jpg", OMRSystem.GetMusicSheetBgrImage());
		OMRSystem.SaveSymbolsToFile();
		waitKey(0);
	}

	

	/*ofstream fout;
	fout.open("debug.txt");

	vector < vector <int> > a = OMRSystem.GetIntensityImage();
	for (int i = 0; i < (int)a.size(); i++) 
	{
		for (int j = 0; j < (int)a[i].size(); j++)
			fout << a[i][j] << ' ';
		fout << '\n';
	}

	fout.close();*/

	//waitKey(0);

	return 0;
}