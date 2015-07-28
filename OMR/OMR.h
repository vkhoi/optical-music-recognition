#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <iostream>
#include <string>

#include "ImagePreprocess.h"
#include "StaffDetection.h"
#include "ConnectedComponent.h"
#include "BeamDetection.h"
#include "SymbolDetection.h"
#include "Utilities.h"
#include "Classification.h"

using namespace cv;
using namespace std;

class OMR
{
public:

	OMR(Mat & inputImage);
	~OMR();

	// return the image
	Mat GetMusicSheetImage();

	// return the bgr image
	Mat GetMusicSheetBgrImage();

	// return binarized image
	Mat GetBinarizedImage();

	// return the 2D array contains intensity of image
	vector < vector <int> > GetIntensityImage();

	// save symbols
	void SaveSymbolsToFile();

private:

	// image of the music sheet
	Mat image, binarizedImage, bgrImage;

	// 2D array contains intensity of each pixel
	vector < vector <int> > vImage;

	// 2D array contains colors of each pixel in 24-bit
	vector < vector <int> > vBgrImage;

	vector <StaffLine> staffLines;
	vector <StaffLine> mergedStaffLines;
	vector <Beam> beams;
	vector <Stem> stems;
	vector <Symbol> symbols;
	vector <Symbol> symbolsToShow;

	int staffLineHeight;
	int staffSpaceHeight;

	/*---------------------------------------------*/

	// get intensity of image
	void getIntensity();

	// binarize the image
	void binarization();

	// return BGR Mat of intensity
	void makeBgrImage();

	// calculate staff line height and staff space height
	void getStaffHeight();

	// detect staff lines
	void detectStaffLines();

	// segmentation
	void segmentation();

	// get symbols
	void getSymbols();

	// classify symbols
	void classifySymbols();

	// update intensity from 2D array into Mat
	void updateIntoMat();

	// update pixel of 2D array of bgr Mat
	void updateIntoBgrMat();

	// color beams
	void colorBeams(vector <Beam> & beams, int color);

	// color stems
	void colorStems(vector <Stem> & stems, int color);

	// color beams
	void colorBeamsBgr(vector <Beam> & beams, int color);

	// color stems
	void colorStemsBgr(vector <Stem> & stems, int color);

	// filetr stems
	void filterStems();

	// show symbols
	void showSymbols();
};

