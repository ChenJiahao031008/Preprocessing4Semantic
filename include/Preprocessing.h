#include<iostream>
#include<string>
#include<opencv2/opencv.hpp>

using namespace std;

class Preprocessing
{
public:
	Preprocessing(const string &strPathToSequence);
	//~Preprocessing();

	void LabelmeTool();

	int LoadImg();
	
	void Random(const double &percentage);

	void SimpleDeformation(const double &percentage);

	void Resize(const double &percentage, const double &minScale,const double &maxScale);

	void Tailor(const double &percentage);

	void IlluminationAndContrast(const double &percentage);

	void Saturation(const double &percentage);

	void Mosaic(const double &percentage);

	void RandomErasure(const double &percentage);

private:

	string mstrPathToSequence;

	std::vector<cv::String> mvstrPictureNames, mvstrSemanticNames;

	std::vector<cv::Mat> mvmatImages, mvmatSemantics;

	std::vector<int> mvichangeIdx;
};