#include "StaffLine.h"

#define BLACK 0
#define WHITE 255
#define oo 100000

StaffLine::StaffLine()
{
	minY = oo;
	maxY = -1;
}


StaffLine::~StaffLine()
{
	pixels.clear();
	colors.clear();
}

/*----------------------------------------------*/

void StaffLine::Add(Point2i pixel, int color) 
{
	started = true;
	if (!started && color == WHITE)
		return;

	started = true;
	pixels.push_back(pixel);
	colors.push_back(color);
	if (color == BLACK)
		nBlackPixels++;
	else
		nWhitePixels++;

	sumX += pixel.x;
	meanX = sumX / (double)pixels.size();

	minY = min(minY, pixel.y);
	maxY = max(maxY, pixel.y);
}

double StaffLine::GetPercentBlackPixels() 
{
	if (pixels.size() == 0) return -1;
	return nBlackPixels * 1.0 / pixels.size();
}

void StaffLine::TrimEnd()
{
	while (colors.size() > 0 && colors[colors.size() - 1] == WHITE)
	{
		pixels.pop_back();
		colors.pop_back();
	}
}

Point2i StaffLine::GetPixel(int i)
{
	return pixels[i];
}

int StaffLine::GetColor(int i)
{
	return colors[i];
}

int StaffLine::GetNumerPixels()
{
	return pixels.size();
}

double StaffLine::GetMeanX()
{
	return meanX;
}

int StaffLine::GetMinY()
{
	return minY;
}

int StaffLine::GetMaxY()
{
	return maxY;
}

/*----------------------------------------------*/

