#include "BeamDetection.h"

#define oo 1000000
#define WHITE 255
#define BLACK 0

const int mov[8][2] = { { -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, -1 }, { 0, 1 }, { 1, -1 }, { 1, 0 }, { 1, 1 } };

BeamDetection::BeamDetection(vector <Point2i> comp, int staffLineHeight, int staffSpaceHeight)
{
	this->comp = comp;
	this->staffLineHeight = staffLineHeight;
	this->staffSpaceHeight = staffSpaceHeight;
	makevBoard();

	if (minX == 395 && minY == 699)
	{
		int t = 0;
		t++;
		t--;
	}

	// find stems and remove
	StemDetection stemDetection(staffLineHeight, staffSpaceHeight, vBoard);
	stems = stemDetection.GetStems();
	filteredStems = stemDetection.GetFilteredStems();
	cerr << "...Erasing stems..." << endl;
	colorStems(filteredStems, 1);

	if (filteredStems.size() == 1)
		return;

	// find beams
	findBeams();
}


BeamDetection::~BeamDetection()
{
}

vector <Stem> BeamDetection::GetStems()
{
	vector <Stem> result = stems;
	for (int i = 0; i < result.size(); i++)
		result[i].Translate(minX, minY);
	return result;
}

vector <Stem> BeamDetection::GetFilteredStems()
{
	vector <Stem> result = filteredStems;
	for (int i = 0; i < result.size(); i++)
		result[i].Translate(minX, minY);
	return result;
}

vector <Beam> BeamDetection::GetBeams()
{
	vector <Beam> result = beams;
	for (int i = 0; i < result.size(); i++)
		result[i].Translate(minX, minY);
	return result;
}

int BeamDetection::GetMinX()
{
	return minX;
}

int BeamDetection::GetMinY()
{
	return minY;
}

/*----------------------------------------------*/

void BeamDetection::makevBoard()
{
	int minx = oo, maxx = -1, miny = oo, maxy = -1;
	for (int i = 0; i < comp.size(); i++)
	{
		Point2i p = comp[i];
		minx = min(minx, p.x);
		maxx = max(maxx, p.x);
		miny = min(miny, p.y);
		maxy = max(maxy, p.y);
	}

	int height = maxx - minx + 1;
	int width = maxy - miny + 1;
	vBoard = vector < vector <int> >(height, vector <int>(width, WHITE));
	
	for (int i = 0; i < comp.size(); i++)
		vBoard[comp[i].x - minx][comp[i].y - miny] = BLACK;
	this->minX = minx;
	this->minY = miny;
}

void BeamDetection::colorStems(vector <Stem> & stems, int color)
{
	for (int i = 0; i < stems.size(); i++)
	{
		for (int j = 0; j < stems[i].GetNumPixels(); j++)
		{
			Point2i p = stems[i].GetPixel(j);
			vBoard[p.x][p.y] = color;
		}
	}
}

void BeamDetection::findBeams()
{
	cerr << "...Finding beams..." << endl;
	if (minX == 529 && minY == 1106)
	{
		int t = 0;
		t++;
		t--;
	}
	vector < vector <Point2i> > comps = ConnectedComponent::GetConnectedComponents(vBoard);
	for (int i = 0; i < comps.size(); i++)
	{
		if (isBeam(comps[i]))
		{
			Beam beam;
			for (int j = 0; j < comps[i].size(); j++)
				beam.AddPixel(comps[i][j]);
			beams.push_back(beam);
		}
	}
}

bool BeamDetection::isBeam(vector <Point2i> & comp)
{
	int x0 = oo, y0 = oo;
	int x1 = -oo, y1 = -oo;
	bool isAdjacentToStem = false;

	for (int i = 0; i < comp.size(); i++)
	{
		Point2i p = comp[i];
		x0 = min(x0, p.x);
		y0 = min(y0, p.y);
		x1 = max(x1, p.x);
		y1 = max(y1, p.y);

		if (!isAdjacentToStem)
			for (int j = 0; j < 8; j++)
			{
				Point2i q(p.x + mov[j][0], p.y + mov[j][1]);
				if (isInside(q.x, q.y) && vBoard[q.x][q.y] == 1)
					isAdjacentToStem = true;
			}
	}

	return y1 - y0 >= 2 * staffSpaceHeight + 2.5 * staffLineHeight
		&& x1 - x0 <= 4 * staffSpaceHeight
		&& isAdjacentToStem;
}

bool BeamDetection::isInside(int x, int y)
{
	return x >= 0 && y >= 0 && x < vBoard.size() && y < vBoard[0].size();
}