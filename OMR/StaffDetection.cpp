#include "StaffDetection.h"

#define BLACK 0
#define WHITE 255
#define BLACK_PERCENT_THRESHOLD 0.55
#define oo 1000000

const int mov[6][2] = { { -1, -1 }, { 0, -1 }, { 1, -1 }, { -1, 1 }, { 0, 1 }, { 1, 1 } };

StaffDetection::StaffDetection(vector < vector <int> > & vImage, int staffLineHeight, int staffSpaceHeight)
{
	this->vImage = vImage;
	this->staffLineHeight = staffLineHeight;
	this->staffSpaceHeight = staffSpaceHeight;

	findStaffLines();
}


StaffDetection::~StaffDetection()
{
}

vector <StaffLine> StaffDetection::GetStaffLines()
{
	return staffLines;
}

vector <StaffLine> StaffDetection::GetMergedStaffLines()
{
	return mergedStaffLines;
}

void StaffDetection::RemoveStaffLines(vector < vector <int> > & vImage, vector < vector <int> > * vBgrImage)
{
	int nRows = vImage.size(), nCols = vImage[0].size();
	int threshold = staffLineHeight + 1;
	int tolerance = (int)(1 + ceil(staffLineHeight / 3.0));
	bool flag = false;

	for (int i = 0; i < staffLines.size(); i++)
	{
		int color1 = rand() % 32768, color2 = rand() % 32768;
		int color = (long long)color1 * color2 % 16777216;
		for (int j = 0; j < staffLines[i].GetNumerPixels(); j++)
		{
			Point2i pixel = staffLines[i].GetPixel(j);
			if (vImage[pixel.x][pixel.y] == WHITE)
			{
				int dist1 = 0, dist2 = 0;
				while (pixel.x - dist1 > -1 && vImage[dist1][pixel.y] == 255 && dist1 <= tolerance)
					dist1++;
				dist1--;
				while (pixel.x + dist2 < nRows && vImage[dist2][pixel.y] == 255 && dist2 <= tolerance)
					dist2++;
				dist2--;

				if (dist1 <= max(1, min(dist2, tolerance)))
					pixel.x -= dist1;
				else if (dist2 <= max(1, min(dist1, tolerance)))
					pixel.x += dist2;
				else
					continue;
			}

			int dist1 = 0, dist2 = 0;
			while (pixel.x - dist1 - 1 > -1 && vImage[pixel.x - dist1 - 1][pixel.y] == BLACK)
			{
				dist1++;
				if (dist1 + 1 > threshold)
					break;
			}
			if (dist1 + 1 > threshold)
				continue;
			while (pixel.x + dist2 + 1 < nRows && vImage[pixel.x + dist2 + 1][pixel.y] == BLACK)
			{
				dist2++;
				if (dist1 + 1 + dist2 > threshold)
					break;
			}
			if (dist1 + 1 + dist2 > threshold)
				continue;
			for (int j = pixel.x - dist1; j <= pixel.x + dist2; j++)
			{
				vImage[j][pixel.y] = WHITE;
				if (vBgrImage != nullptr)
					//(*vBgrImage)[j][pixel.y] = (1 << 24) - 1;
					(*vBgrImage)[j][pixel.y] = color;
				flag = true;
			}
		}
	}
}

/*----------------------------------------------*/

void StaffDetection::findStaffLines() 
{
	vector < vector <int> > dpLeft(vImage.size(), vector <int>(vImage[0].size(), 0));
	vector < vector <int> > dpRight(vImage.size(), vector <int>(vImage[0].size(), 0));
	vector < vector <int> > traceLeft(vImage.size(), vector <int>(vImage[0].size(), 0));
	vector < vector <int> > traceRight(vImage.size(), vector <int>(vImage[0].size(), 0));
	vector < vector <int> > startLeft(vImage.size(), vector <int>(vImage[0].size(), 0));
	vector < vector <int> > startRight(vImage.size(), vector <int>(vImage[0].size(), 0));

	vector < vector <bool> > visited(vImage.size(), vector <bool>(vImage[0].size(), false));

	cerr << "...Dp from left" << endl;
	dpFromLeft(dpLeft, traceLeft, startLeft);
	cerr << "...Dp from right" << endl;
	dpFromRight(dpRight, traceRight, startRight);

	int nRows = dpLeft.size(), nCols = dpLeft[0].size();

	cerr << "...Looking for stable paths" << endl;
	for (int i = 0; i < nRows; i++)
	{
		//cerr << "...Row " << i << endl;
		int rightEnd = startRight[i][0];
		int leftEnd = startLeft[rightEnd][nCols - 1];
		//int leftEnd = startLeft[i][0];
		//int rightEnd = startRight[leftEnd][0];
		if (i == 341)
		{
			int t = 0;
			t++;
			t--;
		}
		if (leftEnd != i) continue;

		StaffLine staff;
		bool ok = true;
		for (int j = 0, y = i; j < nCols; j++)
		{
			if (visited[y][j]) {
				ok = false; break;
			}
			visited[y][j] = true;
			staff.Add(Point2i(y, j), vImage[y][j]);
			y = traceRight[y][j];
		}
		if (!ok) continue;

		if (isValidStaffLine(staff))
		{
			//staff.TrimEnd();
			staffLines.push_back(staff);
		}
	}

	cerr << "...Sorting detected staff lines" << endl;
	sort(staffLines.begin(), staffLines.end(), StaffDetection::blackPixelsCmp);
	mergeStaffLines();
}

void StaffDetection::dpFromLeft(vector < vector <int> > & dpLeft,
	vector < vector <int> > & traceLeft,
	vector < vector <int> > & startLeft)
{
	int nRows = dpLeft.size(), nCols = dpLeft[0].size();
	for (int i = 0; i < nRows; i++)
	{
		dpLeft[i][0] = traceLeft[i][0] = 0;
		startLeft[i][0] = i;
	}

	for (int j = 1; j < nCols; j++)
		for (int i = 0; i < nRows; i++)
		{
			dpLeft[i][j] = oo;
			for (int k = 0; k < 3; k++) 
			{
				int ii = i + mov[k][0], jj = j + mov[k][1];
				if (!isInside(ii, jj)) continue;
				int weight = 2 + (vImage[i][j] == 0 ? 1 : 2) + (vImage[ii][jj] == 0 ? 1 : 2)
					+ (k == 1 ? 0 : 2);
				if (dpLeft[ii][jj] + weight < dpLeft[i][j] || (dpLeft[ii][jj] + weight == dpLeft[i][j] && k == 1))
				{
					dpLeft[i][j] = dpLeft[ii][jj] + weight;
					traceLeft[i][j] = ii;
					startLeft[i][j] = startLeft[ii][jj];
				}
			}
		}
}

void StaffDetection::dpFromRight(vector < vector <int> > & dpRight,
	vector < vector <int> > & traceRight,
	vector < vector <int> > & startRight)
{
	int nRows = dpRight.size(), nCols = dpRight[0].size();
	for (int i = 0; i < nRows; i++)
	{
		dpRight[i][nCols - 1] = traceRight[i][nCols - 1] = 0;
		startRight[i][nCols - 1] = i;
	}

	for (int j = nCols - 2; j > -1; j--)
		for (int i = 0; i < nRows; i++)
		{
			dpRight[i][j] = oo;
			for (int k = 3; k < 6; k++)
			{
				int ii = i + mov[k][0], jj = j + mov[k][1];
				if (!isInside(ii, jj)) continue;
				int weight = 2 + (vImage[i][j] == 0 ? 1 : 2) + (vImage[ii][jj] == 0 ? 1 : 2)
					+ (k == 4 ? 0 : 2);
				if (dpRight[ii][jj] + weight < dpRight[i][j] || (dpRight[ii][jj] + weight == dpRight[i][j] && k == 4))
				{
					dpRight[i][j] = dpRight[ii][jj] + weight;
					traceRight[i][j] = ii;
					startRight[i][j] = startRight[ii][jj];
				}
			}
		}
}

bool StaffDetection::isInside(int x, int y) 
{
	return x >= 0 && y >= 0 && x < vImage.size() && y < vImage[0].size();
}

bool StaffDetection::isValidStaffLine(StaffLine & staff)
{
	return staff.GetPercentBlackPixels() >= BLACK_PERCENT_THRESHOLD;
}

bool StaffDetection::blackPixelsCmp(StaffLine & a, StaffLine & b)
{
	return a.GetPercentBlackPixels() > b.GetPercentBlackPixels();
}

bool StaffDetection::meanXStaffLineCmp(StaffLine & a, StaffLine & b)
{
	return a.GetMeanX() < b.GetMeanX();
}

void StaffDetection::mergeStaffLines()
{
	vector <bool> check(staffLines.size(), false);
	for (int i = 0; i < staffLines.size(); i++)
	{
		if (check[i]) continue;
		mergedStaffLines.push_back(staffLines[i]);
		StaffLine * pStaff = &mergedStaffLines[mergedStaffLines.size() - 1];
		for (int j = i + 1; j < staffLines.size(); j++)
		{
			if (check[j]) continue;
			if (isAdjacentStaffLine(*pStaff, staffLines[j]))
			{
				check[j] = true;
				for (int k = 0; k < staffLines[j].GetNumerPixels(); k++)
				{
					Point2i p = staffLines[j].GetPixel(k);
					pStaff->Add(p, vImage[p.x][p.y]);
				}
			}
		}
	}
	sort(mergedStaffLines.begin(), mergedStaffLines.end(), StaffDetection::meanXStaffLineCmp);
}

bool StaffDetection::isAdjacentStaffLine(StaffLine & a, StaffLine & b)
{
	double meanA = a.GetMeanX(), meanB = b.GetMeanX();
	return abs(meanA - meanB) <= staffSpaceHeight * 0.5;
	for (int i = 0; i < a.GetNumerPixels(); i++)
		for (int j = 0; j < b.GetNumerPixels(); j++)
		{
			Point2i p1 = a.GetPixel(i);
			Point2i p2 = b.GetPixel(j);
			if (abs(p1.x - p2.x) + abs(p1.y - p2.y) <= 1)
				return true;
		}
	return false;
}