#include "Classification.h"

#define NUM_LABELS 14
#define FEATURE_HEIGHT 10
#define FEATURE_WIDTH 12
#define PROB 0.8
#define NUM_SVM 10
#define THRESHOLD 0.8

Classification::Classification(string trainDataDir)
{
	this->trainDataDir = trainDataDir;
	train();
}


Classification::~Classification()
{
}

pair <int, int> Classification::PredictImage(Mat img)
{
	return PredictFeature(Utilities::ToFeature(img));
}

pair <int, int> Classification::PredictFeature(Mat feature)
{
	vector <int> results(NUM_SVM);
	vector <int> cnt(NUM_LABELS, 0);
	int res = 0, maxCnt = 0;
	for (int i = 0; i < NUM_SVM; i++)
	{
		results[i] = svms[i].predict(feature);
		cnt[results[i]]++;
		if (cnt[results[i]] > maxCnt)
		{
			res = results[i];
			maxCnt = cnt[results[i]];
		}
	}
	if ((double)maxCnt / (double)NUM_SVM >= THRESHOLD) return make_pair(res, maxCnt);
	return make_pair(NUM_LABELS, maxCnt);
}

/*----------------------------------------------*/

void Classification::getFileNames()
{
	cerr << "...Getting all files' names and images" << endl;
	DIR * dir;
	dirent * pdir;
	for (int i = 0; i < NUM_LABELS; i++)
	{
		string s_dir = trainDataDir + Utilities::Padding(Utilities::ToString(i), 2);
		dir = opendir(s_dir.c_str());
		vector <string> label;
		vector <Mat> images;
		while (pdir = readdir(dir))
		{
			string name = pdir->d_name;
			if (name.length() < 4 || name.substr(name.length() - 3, 3) != "png") continue;
			string dir = s_dir + "/" + name;
			label.push_back(dir);
			Mat img;
			cvtColor(imread(dir), img, CV_BGR2GRAY);
			Mat resizedImg(FEATURE_HEIGHT, FEATURE_WIDTH, CV_8UC1);
			resize(img, resizedImg, resizedImg.size(), 0, 0, CV_INTER_AREA);
			images.push_back(Utilities::ToFeature(resizedImg));
		}
		fileNames.push_back(label);
		allImages.push_back(images);
	}
}

void Classification::train()
{
	cerr << "...Training" << endl;
	svms.resize(NUM_SVM);
	getFileNames();
	for (int idxSVM = 0; idxSVM < NUM_SVM; idxSVM++)
	{
		cerr << "...Training SVM " << Utilities::ToString(idxSVM) << endl;
		/*svms[idxSVM].load(("svm" + Utilities::ToString(idxSVM)).c_str());
		continue;*/
		vector <Mat> featureForThisSVM;
		vector <int> labelForThisSVM;
		for (int i = 0; i < NUM_LABELS; i++)
		{
			int numFiles = fileNames[i].size();
			int numFilesToTrain = numFiles * PROB;
			vector <bool> check(numFiles, false);
			for (int j = 0; j < numFilesToTrain; j++)
			{
				int x = rand() % numFiles;
				while (check[x])
				{
					x = rand() % numFiles;
				}
				check[x] = true;
				Mat t = allImages[i][x];
				featureForThisSVM.push_back(allImages[i][x]);
				labelForThisSVM.push_back(i);
			}
		}
		Mat feature(featureForThisSVM.size(), FEATURE_HEIGHT * FEATURE_WIDTH, CV_32FC1);
		for (int i = 0; i < featureForThisSVM.size(); i++)
			for (int j = 0; j < FEATURE_HEIGHT * FEATURE_WIDTH; j++)
				feature.at<float>(i, j) = featureForThisSVM[i].at<float>(j, 0);
		Mat label(labelForThisSVM.size(), 1, CV_32FC1);
		for (int i = 0; i < labelForThisSVM.size(); i++)
			label.at<float>(i, 0) = labelForThisSVM[i];

		CvSVMParams params;
		params.svm_type = CvSVM::C_SVC;
		params.kernel_type = CvSVM::LINEAR;
		//params.gamma = 3;
		params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 300, 1e-6);

		svms[idxSVM].train(feature, label, Mat(), Mat(), params);
		svms[idxSVM].save(("svm" + Utilities::ToString(idxSVM)).c_str());
	}
}

vector <int> Classification::getFeature(Mat & img)
{
	int nRows = img.rows, nCols = img.cols;
	vector <int> result(nRows * nCols, 0);
	int n = 0;
	for (int i = 0; i < nRows; i++)
		for (int j = 0; j < nCols; j++)
		{
			int intensity = img.at<uchar>(i, j);
			result[n++] = intensity;
		}
	return result;
}
