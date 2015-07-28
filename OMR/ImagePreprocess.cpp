#include "ImagePreprocess.h"

#define WHITE 255
#define BLACK 0

ImagePreprocess::ImagePreprocess()
{
}


ImagePreprocess::~ImagePreprocess()
{
}

void ImagePreprocess::Otsu(vector < vector <int> > & vImage)
{
	int histogram[256];
	for (int i = 0; i < 256; i++)
		histogram[i] = 0;

	int total = 0, nRows = vImage.size(), nCols = vImage[0].size();
	for (int i = 0; i < nRows; i++)
		for (int j = 0; j < nCols; j++)
		{
			++histogram[vImage[i][j]];
			++total;
		}

	int sum = 0;
	for (int i = 1; i < 256; i++)
		sum += i * histogram[i];

	double sumB = 0;
	double wB = 0;
	double wF = 0;
	double mB;
	double mF;
	double max = 0;
	double between;
	int threshold = 0;

	for (int i = 0; i < 256; i++)
	{
		wB += histogram[i];
		if (abs(wB)<1e-9)
			continue;
		wF = total - wB;
		if (abs(wF)<1e-9)
			break;
		sumB += i * histogram[i];
		mB = sumB / wB;
		mF = (sum - sumB) / wF;
		between = wB * wF * (mB - mF) * (mB - mF);

		if (between > max)
		{
			max = between;
			threshold = i;
		}
	}
	for (int i = 0; i < nRows; i++)
		for (int j = 0; j < nCols; j++)
			if (vImage[i][j] <= threshold)
				vImage[i][j] = 0;
			else
				vImage[i][j] = 255;
}

pair <int, int> ImagePreprocess::GetStaffHeight(vector < vector <int> > & vImage)
{
	int nRows = vImage.size(), nCols = vImage[0].size();
	vector <int> histoBlack(nRows + 1, 0);
	vector <int> histoWhite(nRows + 1, 0);

	for (int j = 0; j < nCols; j++)
	{
		int prev = vImage[0][j], black = 0, white = 0;
		for (int i = 1; i < nRows; i++)
		{
			int value = vImage[i][j];
			if (value != prev)
			{
				if (prev == WHITE)
					histoWhite[white]++;
				else
					histoBlack[black]++;
				black = white = 0;
			}

			if (value == BLACK)
				black++;
			else
				white++;

			prev = value;
		}

		if (prev == WHITE)
			histoWhite[white]++;
		else
			histoBlack[black]++;
	}

	int maxBlack = histoBlack[1], maxWhite = histoWhite[1];
	pair <int, int> result = pair<int, int>(1, 1);
	for (int i = 2; i <= nRows; i++)
	{
		if (histoBlack[i] > maxBlack)
			maxBlack = histoBlack[i], result.first = i;
		if (histoWhite[i] > maxWhite)
			maxWhite = histoWhite[i], result.second = i;
	}

	return result;
}