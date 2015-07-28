#include "Symbol.h"

#define oo 100000

const int mov[8][2] = { { -1, -1, }, { -1, 0 }, { -1, 1 }, { 0, -1 }, { 0, 1 }, { 1, -1 }, { 1, 0 }, { 1, 1 } };

Symbol::Symbol()
{
	x0 = oo, y0 = oo;
	x1 = -oo, y1 = -oo;
}


Symbol::~Symbol()
{
}

void Symbol::AddPixel(Point2i p)
{
	pixels.push_back(p);
	x0 = min(x0, p.x);
	x1 = max(x1, p.x);
	y0 = min(y0, p.y);
	y1 = max(y1, p.y);
}

int Symbol::GetNumPixels()
{
	return pixels.size();
}

Point2i Symbol::GetPixel(int i)
{
	return pixels[i];
}

bool Symbol::IsSymbol(int staffLineHeight)
{
	return true;
}

vector <Point2i> Symbol::GetAllPixels()
{
	return pixels;
}

#include <conio.h>

bool Symbol::IsNote(int staffLineHeight, vector < vector <int> > & vImage)
{
	bool isAdjacentToStem = false;
	if (x0 == 1106 && y0 == 529)
	{
		isAdjacentToStem = false;
	}

	for (int i = 0; i < pixels.size(); i++)
	{
		Point2i p = pixels[i];
		for (int j = 0; j < 8 && !isAdjacentToStem; j++)
		{
			int x = p.x + mov[j][0], y = p.y + mov[j][1];
			if (isInside(x, y, vImage.size(), vImage[0].size()) && vImage[x][y] == 1)
				isAdjacentToStem = true;
		}
	}
	if (x0 == 463 && y0 == 612)
	{
		//cerr << "adjacent to stem = " << isAdjacentToStem << endl;
		//_getch();
	}
	int width = y1 - y0 + 1, height = x1 - x0 + 1;
	if (width >= height) return false;
	if ((double)height >= 5 * width) return false;
	return isAdjacentToStem;
}

bool Symbol::IsWholeNote(int staffSpaceHeight, vector < vector <int> > & vImage)
{
	int width = y1 - y0, height = x1 - x0;
	if (height > width) return false;
	if ((double)height < (double)staffSpaceHeight * 0.9 || (double)height >(double)staffSpaceHeight * 1.6)
		return false;
	return true;
}

bool Symbol::IsFarFromStaff(int staffSpaceHeight, int label, vector <StaffLine> & s)
{
	double C;
	if (x0 == 395 && y0 == 699)
	{
		int t = 0;
		t++;
		t--;
	}
	if (label <= 5)
		C = 2.5;
	else if (label <= 9)
		C = 0.5;
	else
		C = 2.5;
	int n = s.size();
	for (int i = 0; i < n / 5; i++)
	{
		if (y0 >= s[i * 5].GetMinY() - staffSpaceHeight &&
			y0 <= s[i * 5].GetMaxY() - staffSpaceHeight &&
			x1 >= s[i * 5].GetMeanX() - C * staffSpaceHeight &&
			x0 <= s[i * 5 + 4].GetMeanX() + C * staffSpaceHeight)
			return false;
	}
	return true;
}

bool Symbol::IsGClef(int staffSpaceHeight)
{
	int height = x1 - x0 + 1;
	return height >= 5 * staffSpaceHeight;
}

bool Symbol::IsAccidental(int staffSpaceHeight)
{
	int height = x1 - x0 + 1;
	return height >= 1.5 * staffSpaceHeight && height <= 4 * staffSpaceHeight;
}

bool Symbol::IsRest(int staffSpaceHeight)
{
	int height = x1 - x0 + 1, width = y1 - y0 + 1;
	if (width >= height) return false;
	if (height < 2 * staffSpaceHeight) return false;
	return true;
}

/*----------------------------------------------*/

bool Symbol::isInside(int x, int y, int rows, int cols)
{
	return x >= 0 && y >= 0 && x < rows && y < cols;
}

