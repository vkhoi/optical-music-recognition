#include "ConnectedComponent.h"

#define WHITE 255
#define BLACK 0

const int mov[8][2] = { { -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, -1 }, { 0, 1 }, { 1, -1 }, { 1, 0 }, { 1, 1 } };

ConnectedComponent::ConnectedComponent()
{
}


ConnectedComponent::~ConnectedComponent()
{
}

vector < vector <Point2i> > ConnectedComponent::GetConnectedComponents(vector < vector <int> > & vImage)
{
	int nRows = vImage.size(), nCols = vImage[0].size();
	vector < vector <bool> > visited(nRows, vector <bool>(nCols, false));
	vector < vector <Point2i> > result;

	for (int i = 0; i < nRows; i++)
		for (int j = 0; j < nCols; j++)
		{
			if (visited[i][j] || !isInside(i, j, nRows, nCols) || vImage[i][j] != BLACK) continue;
			vector <Point2i> comp = bfs(vImage, visited, i, j);
			result.push_back(comp);
		}

	return result;
}

/*----------------------------------------------*/

bool ConnectedComponent::isInside(int i, int j, int nRows, int nCols)
{
	return i >= 0 && j >= 0 && i < nRows && j < nCols;
}

vector <Point2i> ConnectedComponent::bfs(vector < vector <int> > & vImage, vector < vector <bool> > & visited, int i, int j)
{
	int nRows = vImage.size(), nCols = vImage[0].size();

	vector <Point2i> result;
	int fr = 0, re = 0;
	result.push_back(Point2i(i, j));
	visited[i][j] = true;
	while (fr <= re)
	{
		Point2i p = result[fr++];
		for (int i = 0; i < 8; i++)
		{
			Point2i q(p.x + mov[i][0], p.y + mov[i][1]);
			if (!isInside(q.x, q.y, nRows, nCols) || vImage[q.x][q.y] != BLACK || visited[q.x][q.y]) continue;
			result.push_back(q);
			visited[q.x][q.y] = true;
			re++;
		}
	}

	return result;
}