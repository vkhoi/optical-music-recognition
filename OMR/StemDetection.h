#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <iostream>
#include <queue>

#include "Stem.h"

using namespace cv;
using namespace std;

class StemDetection
{
public:
	StemDetection(int staffLineHeight, int staffSpaceHeight, vector < vector <int> > & vBoard);
	~StemDetection();

	vector <Stem> GetStems();
	vector <Stem> GetFilteredStems();

private:
	int staffLineHeight, staffSpaceHeight;
	vector < vector <int> > vBoard;
	vector <Stem> stems;
	vector <Stem> filteredStems;

	void findStems();
	void mergeStems();
	void filterStems();
	int getMaximumStemWidth();

	bool isStem(Stem & stem);

	int maximumCanGo(int x, int y, int tolerance);
};

