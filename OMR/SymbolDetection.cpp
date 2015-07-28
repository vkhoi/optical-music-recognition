#include "SymbolDetection.h"


SymbolDetection::SymbolDetection(vector < vector <int> > & vImage, int staffLineHeight, int staffSpaceHeight)
{
	this->vImage = vImage;
	this->staffLineHeight = staffLineHeight;
	this->staffSpaceHeight = staffSpaceHeight;

	findSymbols();
}


SymbolDetection::~SymbolDetection()
{
}

vector <Symbol> SymbolDetection::GetSymbols()
{
	return symbols;
}

/*----------------------------------------------*/

void SymbolDetection::findSymbols()
{
	cerr << "Finding symbols..." << endl;
	vector < vector <Point2i> > comps = ConnectedComponent::GetConnectedComponents(vImage);
	for (int i = 0; i < comps.size(); i++)
	{
		Symbol symbol;
		for (int j = 0; j < comps[i].size(); j++)
		{
			Point2i p = comps[i][j];
			symbol.AddPixel(p);
		}
		if (symbol.IsSymbol(staffLineHeight))
			symbols.push_back(symbol);
	}
}