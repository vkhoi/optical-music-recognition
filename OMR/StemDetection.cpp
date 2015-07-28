#include "StemDetection.h"

#define WHITE 255
#define BLACK 0

StemDetection::StemDetection(int staffLineHeight, int staffSpaceHeight, vector < vector <int> > & vBoard)
{
	this->staffLineHeight = staffLineHeight;
	this->staffSpaceHeight = staffSpaceHeight;
	this->vBoard = vBoard;

	findStems();
}


StemDetection::~StemDetection()
{
}

vector <Stem> StemDetection::GetStems()
{
	return stems;
}

vector <Stem> StemDetection::GetFilteredStems()
{
	return filteredStems;
}

/*----------------------------------------------*/

void StemDetection::findStems()
{
	cerr << "...Finding stems..." << endl;
	int nRows = vBoard.size(), nCols = vBoard[0].size();
	for (int j = 0; j < nCols; j++)
	{
		for (int i = 0, len = 0, start = 0; i <= nRows; i++)
		{
			if (i == nRows || vBoard[i][j] == WHITE)
			{
				if (len >= 2 * staffSpaceHeight)
				{
					Stem stem;
					for (int k = start; k < i; k++)
						stem.AddPixel(Point2i(k, j));
					if (isStem(stem))
						stems.push_back(stem);
				}
				len = 0;
				start = i + 1;
				continue;
			}
			else
				len++;
		}
	}
	mergeStems();
	filterStems();
}

void StemDetection::mergeStems()
{
	cerr << "...Merging stems..." << endl;
	vector <bool> check(stems.size(), false);
	vector <Stem> mergedStems;
	int cnt = 0;
	for (int i = 0; i < stems.size(); i++)
	{
		if (check[i]) continue;
		cnt++;
		for (int j = i + 1; j < stems.size(); j++)
			if (stems[i].MergeStem(stems[j]))
				check[j] = true;
	}
	if (cnt >= 1)
		for (int i = 0; i < stems.size(); i++)
			if (!check[i])
				mergedStems.push_back(stems[i]);
	stems = mergedStems;
}

void StemDetection::filterStems()
{
	cerr << "...Filtering stems..." << endl;
	int maxWidth = getMaximumStemWidth();
	for (int i = 0; i < stems.size(); i++)
	{
		Stem filteredStem = stems[i].FilterStem(maxWidth, vBoard);
		filteredStems.push_back(filteredStem);
	}
}

bool StemDetection::isStem(Stem & stem)
{
	int nRows = vBoard.size(), nCols = vBoard[0].size();
	Point2i minX = stem.GetMinX(), maxX = stem.GetMaxX();
	int height = stem.GetHeight();

	for (int x = minX.x; x <= min(minX.x + height / 4, maxX.x); x++)
	{
		int dist = maximumCanGo(x, minX.y, staffLineHeight);
		if (dist >= 3 * staffLineHeight) return true;
		/*int tolerance = staffLineHeight;
		int xLeft = minX.y, xRight = minX.y;
		while (xLeft >= 0)
		{
			if (vBoard[x][xLeft] == BLACK)
				xLeft--;
			else if (tolerance > 0)
			{
				xLeft--;
				tolerance--;
			}
			else
				break;
		}
		xLeft++;

		while (xRight < nCols)
		{
			if (vBoard[x][xRight] == BLACK)
				xRight++;
			else if (tolerance > 0)
			{
				xRight++;
				tolerance--;
			}
			else
				break;
		}
		xRight--;
		if (xRight - xLeft + 1 >= 3 * staffLineHeight)
			return true;*/
	}

	for (int x = maxX.x; x >= max(maxX.x - height / 4, minX.x); x--)
	{
		int dist = maximumCanGo(x, maxX.y, staffLineHeight);
		if (dist >= 3 * staffLineHeight) return true;
		/*int tolerance = staffLineHeight;
		int xLeft = maxX.y, xRight = maxX.y;
		while (xLeft >= 0)
		{
			if (vBoard[x][xLeft] == BLACK)
				xLeft--;
			else if (tolerance > 0)
			{
				xLeft--;
				tolerance--;
			}
			else
				break;
		}
		xLeft++;

		while (xRight < nCols)
		{
			if (vBoard[x][xRight] == BLACK)
				xRight++;
			else if (tolerance > 0)
			{
				xRight++;
				tolerance--;
			}
			else
				break;
		}
		xRight--;
		if (xRight - xLeft + 1 >= 3 * staffLineHeight)
			return true;*/
	}

	return false;
}

int StemDetection::getMaximumStemWidth()
{
	int width = -1;
	for (int i = 0; i < stems.size(); i++)
	{
		width = max(width, stems[i].GetWidth());
	}
	return width;
}

const int mov[6][2] = { { -1, -1 }, { -1, 1 }, { 0, -1 }, { 0, 1 }, { 1, -1 }, { 1, 1 } };

int StemDetection::maximumCanGo(int x, int y, int tolerance)
{
	int nCols = vBoard[0].size(), nRows = vBoard.size(), res = -1;
	int left = y, right = y, tol = tolerance;
	while (left >= 0)
	{
		if (vBoard[x][left] == BLACK)
			left--;
		else if (tol > 0)
		{
			left--;
			tol--;
		}
		else
			break;
	}
	left++;
	while (right < nCols)
	{
		if (vBoard[x][right] == BLACK)
			right++;
		else if (tol > 0)
		{
			right++;
			tol--;
		}
		else break;
	}
	right--;
	res = max(right - left + 1, res);

	left = y, right = y, tol = tolerance;
	int top = x, bot = x;
	while (left >= 0 && top >= 0)
	{
		if (vBoard[top][left] == BLACK)
			left--, top--;
		else if (tol > 0)
		{
			left--, top--;
			tol--;
		}
		else break;
	}
	left++, top++;
	while (right < nCols && bot < nRows)
	{
		if (vBoard[bot][right] == BLACK)
			bot++, right++;
		else if (tol > 0)
		{
			bot++, right++;
			tol--;
		}
		else break;
	}
	bot--, right--;
	res = max(right - left + 1, res);

	left = y, right = y, tol = tolerance;
	top = x, bot = x;
	while (left >= 0 && top < nRows)
	{
		if (vBoard[top][left] == BLACK)
			left--, top++;
		else if (tol > 0)
		{
			left--, top++;
			tol--;
		}
		else break;
	}
	left++, top--;
	while (right < nCols && bot >= 0)
	{
		if (vBoard[bot][right] == BLACK)
			bot--, right++;
		else if (tol > 0)
		{
			bot--, right++;
			tol--;
		}
		else break;
	}
	bot++, right--;
	res = max(right - left + 1, res);

	return res;
}