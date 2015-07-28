#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

#include "StemDetection.h"
#include "ConnectedComponent.h"
#include "Beam.h"

using namespace cv;
using namespace std;

class BeamDetection
{
public:
	BeamDetection(vector <Point2i> comp, int staffLineHeight, int staffSpaceHeight);
	~BeamDetection();

	vector <Stem> GetStems();
	vector <Stem> GetFilteredStems();

	vector <Beam> GetBeams();

	int GetMinX();
	int GetMinY();

private:
	int staffLineHeight, staffSpaceHeight;
	int minX, minY;
	vector <Point2i> comp;
	vector < vector <int> > vBoard;
	vector <Stem> stems;
	vector <Stem> filteredStems;

	vector <Beam> beams;

	// create 2D board
	void makevBoard();

	void colorStems(vector <Stem> & stem, int color);

	void findBeams();
	bool isBeam(vector <Point2i> & comp);

	bool isInside(int x, int y);
};

