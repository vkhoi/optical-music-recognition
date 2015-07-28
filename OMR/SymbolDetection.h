#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <iostream>

#include "ConnectedComponent.h"
#include "Symbol.h"

using namespace std;
using namespace cv;

class SymbolDetection
{
public:
	SymbolDetection(vector < vector <int> > & vImage, int staffLineHeight, int staffSpaceHeight);
	~SymbolDetection();

	vector <Symbol> GetSymbols();

private:
	int staffLineHeight, staffSpaceHeight;
	vector < vector <int> > vImage;
	vector <Symbol> symbols;

	void findSymbols();
};

