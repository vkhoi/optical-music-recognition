#include "Stem.h"

#define WHITE 255
#define BLACK 0
#define oo 10000000

Stem::Stem()
{
	minX = Point2i(-1, -1);
	maxX = Point2i(-1, -1);
	x0 = oo, x1 = -1;
	y0 = oo, y1 = -1;
}


Stem::~Stem()
{
}

void Stem::AddPixel(Point2i p)
{
	pixels.push_back(p);
	if (minX.x == -1 || p.x < minX.x) minX = p;
	if (maxX.x == -1 || p.x > maxX.x) maxX = p;
	x0 = min(x0, p.x);
	x1 = max(x1, p.x);
	y0 = min(y0, p.y);
	y1 = max(y1, p.y);
}

int Stem::GetNumPixels()
{
	return pixels.size();
}

Point2i Stem::GetPixel(int i)
{
	return pixels[i];
}

Point2i Stem::GetMinX()
{
	return minX;
}

Point2i Stem::GetMaxX()
{
	return maxX;
}

bool Stem::MergeStem(Stem & stem)
{
	bool success = false;
	for (int i = 0; i < pixels.size(); i++)
	{
		for (int j = 0; j < stem.GetNumPixels() && !success; j++)
			if (isAdjacent(pixels[i], stem.GetPixel(j)))
				success = true;
	}
	if (success)
	{
		for (int i = 0; i < stem.GetNumPixels(); i++)
			AddPixel(stem.GetPixel(i));
	}
	return success;
}

Stem Stem::FilterStem(int maxWidth, vector < vector <int> > & vBoard)
{
	int nRows = vBoard.size(), nCols = vBoard[0].size();
	Stem filteredStem;

	for (int i = 0; i < pixels.size(); i++)
	{
		int x = pixels[i].x, y = pixels[i].y;
		int y0 = y, y1 = y;
		while (y0 > 0 && vBoard[x][y0 - 1] == BLACK)
			y0--;
		while (y1 < nCols - 1 && vBoard[x][y1 + 1] == BLACK)
			y1++;
		if (y1 - y0 > maxWidth + 1)
			continue;
		filteredStem.AddPixel(pixels[i]);
	}

	return filteredStem;
}

int Stem::GetWidth()
{
	if (y0 == -1) return 0;
	return y1 - y0 + 1;
}

void Stem::Translate(int x, int y)
{
	for (int i = 0; i < pixels.size(); i++)
		pixels[i].x += x, pixels[i].y += y;
	x0 += x; x1 += x;
	y0 += y; y1 += y;
}

int Stem::GetHeight()
{
	if (x0 == -1) return 0;
	return x1 - x0 + 1;
}

bool Stem::IsFarFromStaff(int staffSpaceHeight, vector <StaffLine> & s)
{
	int n = s.size();
	for (int i = 0; i < n / 5; i++)
	{
		if (y0 >= s[i * 5].GetMinY() - staffSpaceHeight &&
			y0 <= s[i * 5].GetMaxY() - staffSpaceHeight &&
			x1 >= s[i * 5].GetMeanX() - 2.5 * staffSpaceHeight &&
			x0 <= s[i * 5 + 4].GetMeanX() + 2.5 * staffSpaceHeight)
			return false;
	}
	return true;
}

/*----------------------------------------------*/

bool Stem::isAdjacent(Point2i a, Point2i b)
{
	return abs(a.x - b.x) + abs(a.y - b.y) <= 1;
}