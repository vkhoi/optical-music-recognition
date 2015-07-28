#include "OMR.h"

#define oo 100000
#define WHITE 255
#define BLACK 0
#define FEATURE_HEIGHT 10
#define FEATURE_WIDTH 12
#define NUM_LABELS 14

OMR::OMR(Mat & inputImage)
{
	// copy the data from input image
	inputImage.copyTo(image);

	// convert to grayscale
	cvtColor(image, image, CV_BGR2GRAY);

	// get intensity of each pixel
	getIntensity();

	// binarization
	binarization();

	// get staff line height and staff space height
	getStaffHeight();

	// detect staff lines
	detectStaffLines();
	return;
	
	// segmentation
	segmentation();
	return;

	// find symbols
	getSymbols();

	// classify
	classifySymbols();
	showSymbols();
}


OMR::~OMR()
{
}

/*----------------------------------------------*/

Mat OMR::GetMusicSheetImage() 
{
	return image;
}

Mat OMR::GetMusicSheetBgrImage()
{
	return bgrImage;
}

Mat OMR::GetBinarizedImage()
{
	return binarizedImage;
}

vector < vector <int> > OMR::GetIntensityImage()
{
	return vImage;
}

void OMR::SaveSymbolsToFile()
{
	cerr << "...Saving symbols' images to files" << endl;
	for (int i = 0; i < symbols.size(); i++)
	{
		int minX = oo, minY = oo, maxX = -1, maxY = -1;
		for (int j = 0; j < symbols[i].GetNumPixels(); j++)
		{
			Point2i p = symbols[i].GetPixel(j);
			minX = min(minX, p.x);
			minY = min(minY, p.y);
			maxX = max(maxX, p.x);
			maxY = max(maxY, p.y);
		}
		int width = maxY - minY + 1, height = maxX - minX + 1;
		Mat img(height, width, CV_8UC1, Scalar(255));
		for (int j = 0; j < symbols[i].GetNumPixels(); j++)
		{
			Point2i p = symbols[i].GetPixel(j);
			img.at<uchar>(p.x - minX, p.y - minY) = BLACK;
		}
		imwrite("data/symbols/" + Utilities::Padding(Utilities::ToString(i), 3) + "_" + Utilities::ToString(minY) + "-" + Utilities::ToString(minX) + ".jpg", img);
	}
}

/*----------------------------------------------*/

void OMR::getIntensity() 
{
	cerr << "Getting intensity..." << endl;
	vImage = vector < vector <int> >(image.rows, vector <int>(image.cols, 0));
	for (int i = 0; i < image.rows; i++)
		for (int j = 0; j < image.cols; j++) 
		{
			vImage[i][j] = image.at<uchar>(i, j);
		}
}

void OMR::makeBgrImage()
{
	bgrImage = Mat::zeros(image.rows, image.cols, CV_8UC3);
	bgrImage.setTo(Scalar(255, 255, 255));

	vBgrImage = vector < vector <int> >(image.rows, vector <int>(image.cols, 16777215));

	for (int i = 0; i < image.rows; i++)
		for (int j = 0; j < image.cols; j++)
		{
			bgrImage.at<Vec3b>(i, j)[0] = bgrImage.at<Vec3b>(i, j)[1] = bgrImage.at<Vec3b>(i, j)[2] = vImage[i][j];
			if (vImage[i][j] == 0)
				vBgrImage[i][j] = 0;
		}
}

void OMR::updateIntoMat() 
{
	for (int i = 0; i < image.rows; i++)
		for (int j = 0; j < image.cols; j++)
			image.at<uchar>(i, j) = vImage[i][j];
}

void OMR::updateIntoBgrMat()
{
	for (int i = 0; i < bgrImage.rows; i++)
		for (int j = 0; j < bgrImage.cols; j++)
		{
			int b = vBgrImage[i][j] & 255;
			int g = (vBgrImage[i][j] >> 8) & 255;
			int r = (vBgrImage[i][j] >> 16) & 255;
			bgrImage.at<Vec3b>(i, j)[0] = b;
			bgrImage.at<Vec3b>(i, j)[1] = g;
			bgrImage.at<Vec3b>(i, j)[2] = r;
		}
}

void OMR::binarization()
{
	cerr << "Binarizing..." << endl;
	ImagePreprocess::Otsu(vImage);
	updateIntoMat();
	makeBgrImage();
	image.copyTo(binarizedImage);
}

void OMR::getStaffHeight()
{
	cerr << "Finding staff line height & staff space height..." << endl;
	pair <int, int> staffHeight = ImagePreprocess::GetStaffHeight(vImage);
	staffLineHeight = staffHeight.first;
	staffSpaceHeight = staffHeight.second;
	cerr << "Staff line height = " << staffLineHeight << endl;
	cerr << "Staff space height = " << staffSpaceHeight << endl;
}

void OMR::detectStaffLines()
{
	cerr << "Detecting staff lines..." << endl;
	StaffDetection staffDetection(vImage, staffLineHeight, staffSpaceHeight);
	staffLines = staffDetection.GetStaffLines();
	mergedStaffLines = staffDetection.GetMergedStaffLines();

	cerr << "Removing staff lines..." << endl;
	staffDetection.RemoveStaffLines(vImage, &vBgrImage);
	updateIntoMat();
	updateIntoBgrMat();
}

void OMR::segmentation()
{
	cerr << "Segmentation..." << endl;
	vector < vector <Point2i> > comps = ConnectedComponent::GetConnectedComponents(vImage);

	cerr << "...Detecting beams..." << endl;
	for (int i = 0; i < comps.size(); i++)
	{
		cerr << "...process comps[" << i << "]" << endl;
		for (int j = 0; j < comps[i].size(); j++)
		{
			Point2i p = comps[i][j];
			if (p.x == 398 && p.y == 724)
			{
				int t = 0;
				t++;
				t--;
			}
		}
		BeamDetection beamDetection(comps[i], staffLineHeight, staffSpaceHeight);
		vector <Beam> beams = beamDetection.GetBeams();
		vector <Stem> stems = beamDetection.GetStems();
		for (int i = 0; i < beams.size(); i++)
		{
			this->beams.push_back(beams[i]);
		}
		for (int i = 0; i < stems.size(); i++)
		{
			this->stems.push_back(stems[i]);
		}
		/*int minX = beamDetection.GetMinX(), minY = beamDetection.GetMinY();
		for (int j = 0; j < beams.size(); j++)
		{
			for (int k = 0; k < beams[j].GetNumPixels(); k++)
			{
				Point2i p = beams[j].GetPixel(k);
				vBgrImage[p.x + minX][p.y + minY] = 255 << 8;
			}
		}*/
	}

	// filter stems
	//filterStems();

	// erase beams
	/*colorBeams(this->beams, 255);
	colorStems(this->stems, 0);*/
	//colorStemsBgr(this->stems, (1 << 24) - 1);
	colorBeamsBgr(this->beams, (1 << 24) - 1);
	colorStemsBgr(this->stems, 0);
	updateIntoBgrMat();
	updateIntoMat();
}

void OMR::colorBeams(vector <Beam> & beams, int color)
{
	for (int i = 0; i < beams.size(); i++)
	{
		for (int j = 0; j < beams[i].GetNumPixels(); j++)
		{
			Point2i p = beams[i].GetPixel(j);
			vImage[p.x][p.y] = color;
		}
	}
}

void OMR::colorStems(vector <Stem> & stems, int color)
{
	for (int i = 0; i < stems.size(); i++)
	{
		for (int j = 0; j < stems[i].GetNumPixels(); j++)
		{
			Point2i p = stems[i].GetPixel(j);
			vImage[p.x][p.y] = color;
		}
	}
}

void OMR::colorBeamsBgr(vector <Beam> & beams, int color)
{
	for (int i = 0; i < beams.size(); i++)
	{
		for (int j = 0; j < beams[i].GetNumPixels(); j++)
		{
			Point2i p = beams[i].GetPixel(j);
			vBgrImage[p.x][p.y] = color;
		}
	}
}

void OMR::colorStemsBgr(vector <Stem> & stems, int color)
{
	for (int i = 0; i < stems.size(); i++)
	{
		for (int j = 0; j < stems[i].GetNumPixels(); j++)
		{
			Point2i p = stems[i].GetPixel(j);
			vBgrImage[p.x][p.y] = color;
		}
	}
}

void OMR::getSymbols()
{
	SymbolDetection symbolDetection(vImage, staffLineHeight, staffSpaceHeight);
	symbols = symbolDetection.GetSymbols();
}

void OMR::classifySymbols()
{
	cerr << "Classifying symbols..." << endl;
	colorStems(this->stems, 1);
	Classification classification("data/train/");
	string s_predict = "data/predict/";
	for (int i = 0; i < symbols.size(); i++)
	{
		vector <Point2i> pixels = symbols[i].GetAllPixels();
		Mat img = Utilities::GetImage(pixels);
		Mat resizedImg(FEATURE_HEIGHT, FEATURE_WIDTH, CV_8UC1);
		resize(img, resizedImg, resizedImg.size(), 0, 0, CV_INTER_AREA);
		pair <int, int> label = classification.PredictImage(resizedImg);
		int guess = label.first;
		if (label.first == 6)
		{
			int t = 0;
			t++;
			t--;
		}
		if (symbols[i].IsFarFromStaff(staffSpaceHeight, label.first, mergedStaffLines))
			label.first = NUM_LABELS;
		else if (label.first <= 4 && !symbols[i].IsNote(staffLineHeight, vImage))
			label.first = NUM_LABELS;
		else if (label.first == 5 && !symbols[i].IsWholeNote(staffSpaceHeight, vImage))
			label.first = NUM_LABELS;
		else if (label.first == 13 && !symbols[i].IsGClef(staffSpaceHeight))
			label.first = NUM_LABELS;		
		else if (label.first >= 10 && label.first <= 12 && !symbols[i].IsAccidental(staffSpaceHeight))
			label.first = NUM_LABELS;
		else if (label.first >= 6 && label.first <= 9 && !symbols[i].IsRest(staffSpaceHeight))
			label.first = NUM_LABELS;
		string dir = s_predict + Utilities::Padding(Utilities::ToString(label.first), 2) + "/" + Utilities::ToString(i) + "_" + Utilities::ToString(guess) + 
			"-" + Utilities::ToString(label.second) + ".jpg";
		if (label.first == 10)
		{
			//symbolsToShow.push_back(symbols[i]);
		}
		imwrite(dir, img);
	}
	colorStems(this->stems, 0);
}

#include <conio.h>

void OMR::filterStems()
{
	cerr << "Filtering stems that do not satisfy required height" << endl;
	cerr << "No. staff lines = " << mergedStaffLines.size() << endl;
	vector <int> cnt(staffSpaceHeight * 10, 0);
	vector <int> height(stems.size(), 0);
	int H = -1, maxCnt = 0;
	for (int i = 0; i < stems.size(); i++)
	{
		if (stems[i].IsFarFromStaff(staffSpaceHeight, mergedStaffLines))
			height[i] = oo;
		else
			height[i] = stems[i].GetHeight();
		if (height[i] >= staffSpaceHeight * 10) continue;
		cnt[height[i]]++;
		if (cnt[height[i]] > maxCnt)
		{
			maxCnt = cnt[height[i]];
			H = height[i];
		}
	}
	cerr << "No. of stems = " << stems.size() << endl;
	cerr << "Most occured height = " << H << endl;
	vector <Stem> filteredStems;
	for (int i = 0; i < stems.size(); i++)
	{
		if (height[i] >= staffSpaceHeight * 10) continue;
		if (height[i] >= 0.7 * H && height[i] <= 2.0 * H)
			filteredStems.push_back(stems[i]);
	}
	stems = filteredStems;
	cerr << "No. of stems (after) = " << stems.size() << endl;
	cerr << "Staff space height = " << staffSpaceHeight << endl;
}

void OMR::showSymbols()
{
	for (int i = 0; i < symbolsToShow.size(); i++)
	{
		for (int j = 0; j < symbolsToShow[i].GetNumPixels(); j++)
		{
			Point2i p = symbolsToShow[i].GetPixel(j);
			vBgrImage[p.x][p.y] = 255 << 8;
		}
	}
	updateIntoBgrMat();
}