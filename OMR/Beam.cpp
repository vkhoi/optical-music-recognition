#include "Beam.h"


Beam::Beam()
{
}


Beam::~Beam()
{
}

void Beam::AddPixel(Point2i p)
{
	pixels.push_back(p);
}

int Beam::GetNumPixels()
{
	return pixels.size();
}

Point2i Beam::GetPixel(int i)
{
	return pixels[i];
}

void Beam::Translate(int x, int y)
{
	for (int i = 0; i < pixels.size(); i++)
		pixels[i].x += x, pixels[i].y += y;
}

/*----------------------------------------------*/
