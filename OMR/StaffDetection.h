#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

#include "StaffLine.h"

using namespace std;
using namespace cv;

class StaffDetection
{
public:
	StaffDetection(vector < vector <int> > & vImage, int staffLineHeight, int staffSpaceHeight);
	~StaffDetection();

	// return list of staff lines
	vector <StaffLine> GetStaffLines();
	vector <StaffLine> GetMergedStaffLines();

	// remove staff lines
	void RemoveStaffLines(vector < vector <int> > & vImage, vector < vector <int> > * vBgrImage = nullptr);

private:
	
	vector < vector <int> > vImage;
	int staffLineHeight, staffSpaceHeight;

	vector <StaffLine> staffLines;
	vector <StaffLine> mergedStaffLines;

	// find staff lines
	void findStaffLines();

	// dp from left/right
	void dpFromLeft(
		vector < vector <int> > & dpLeft,
		vector < vector <int> > & traceLeft,
		vector < vector <int> > & startLeft);
	void dpFromRight(
		vector < vector <int> > & dpRight,
		vector < vector <int> > & traceRight,
		vector < vector <int> > & startRight);

	// return true if (x, y) is inside image
	bool isInside(int x, int y);

	// return true if input staff line is valid
	bool isValidStaffLine(StaffLine & staff);

	// bool comparison function to compare percent of black pixels between staff lines
	static bool blackPixelsCmp(StaffLine & a, StaffLine & b);
	static bool meanXStaffLineCmp(StaffLine & a, StaffLine & b);

	// merge staff lines
	void mergeStaffLines();

	bool isAdjacentStaffLine(StaffLine & a, StaffLine & b);
};

