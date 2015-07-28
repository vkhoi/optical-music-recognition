#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

using namespace std;
using namespace cv;

class ConnectedComponent
{
public:
	ConnectedComponent();
	~ConnectedComponent();

	static vector < vector <Point2i> > GetConnectedComponents(vector < vector <int> > & vImage);

private:
	static bool isInside(int i, int j, int nRows, int nCols);
	static vector <Point2i> bfs(vector < vector <int> > & vImage, vector < vector <bool> > & visited, int i, int j);
};

