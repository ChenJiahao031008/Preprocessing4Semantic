#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <ctime>
#include "Preprocessing.h"

using namespace std;

Preprocessing::Preprocessing(const string &strPathToSequence):mstrPathToSequence(strPathToSequence)
{
}

int Preprocessing::LoadImg()
{
	string strPatternRGB = mstrPathToSequence + "/image_2/*.png";
	string strPatternSem = mstrPathToSequence + "/semantic/*.png";
	cv::glob(strPatternRGB, mvstrPictureNames);
	cv::glob(strPatternSem, mvstrSemanticNames);
	if ( mvstrPictureNames.size()==0 || mvstrSemanticNames.size()==0 ) {
		std::cout << "[ERRO] NO IMAGE FILES[PNG] " << std::endl;
		return -1;
	}

	cout << "[INFO] SIZE OF PICTURES IS : " << mvstrPictureNames.size() << endl;;
 
	for (unsigned int f = 0; f<mvstrPictureNames.size(); ++f) {
		cv::Mat tmpImage = cv::imread(mvstrPictureNames[f]);
		cv::Mat tmpSemantic = cv::imread(mvstrSemanticNames[f]);
		//cout << "RGB: " << mvstrPictureNames[f] << endl;
		//cout << "SEM: " << mvstrSemanticNames[f] << endl;
		mvmatImages.push_back(tmpImage);
		mvmatSemantics.push_back(tmpSemantic);
	}
	return 0;

}

void Preprocessing::Random(const double &percentage)
{
	srand((int)time(0));
	int iChangeNum  = (int)(percentage * mvmatImages.size());
	int iRegionSize = mvmatImages.size() / iChangeNum;
	// cout << "iRegionSize: " << iRegionSize << endl; 
	cout << iChangeNum << endl; 
	mvichangeIdx.clear();
	for(int idx=0, count=0; idx<(int)mvmatImages.size() && count<iChangeNum; idx+=iRegionSize,count++){
		int randomNum = ( rand()%iRegionSize ) + idx; //[idx, idx+iRegionSize)
		mvichangeIdx.push_back( randomNum );
	}
	/*cout << endl;
	for (auto &i:mvichangeIdx){
		cout << i << " ";
	}
	cout << endl;*/
}

void Preprocessing::SimpleDeformation(const double &percentage)
{
	cout << "[INFO] Number Of Pictures Corrected For SimpleDeformation is :" ;
	Random(percentage);

	for(int i1=0; i1<(int)mvichangeIdx.size(); ++i1)
	{
		int i2 = mvichangeIdx[i1];
		cv::Mat tmpImage, tmpSemantic;
		string strPrefix;
		if (i1%3==0){
			flip(mvmatImages[i2],tmpImage,1);
			flip(mvmatSemantics[i2],tmpSemantic,1);
			strPrefix = "SD0_";
		}else if(i1%3==1){
			flip(mvmatImages[i2],tmpImage,0);
			flip(mvmatSemantics[i2],tmpSemantic,0);
			strPrefix = "SD1_";
		}else{
			flip(mvmatImages[i2],tmpImage,-1);
			flip(mvmatSemantics[i2],tmpSemantic,-1);
			strPrefix = "SD2_";
		}

		string strTmp = mvstrPictureNames[i2];
		int pos = strTmp.find_last_of('/');
    	string strFilesName = strTmp.erase(0, pos + 1);
		string strNewRGBName = "../NewRGB/" + strPrefix + strFilesName;
		string strNewSemName = "../NewSem/" + strPrefix + strFilesName;
		cv::imwrite(strNewRGBName,tmpImage);
		cv::imwrite(strNewSemName,tmpSemantic);

		mvstrPictureNames.push_back(strNewRGBName);
		mvstrSemanticNames.push_back(strNewSemName);
		mvmatImages.push_back(tmpImage);
		mvmatSemantics.push_back(tmpSemantic);
	}
	cout << "[INFO] SimpleDeformation Finished! " << endl;
}

void Preprocessing::Resize(const double &percentage,const double &minScale, const double &maxScale)
{
	cout << "[INFO] Number Of Pictures Corrected For Resize is :" ;
	Random(percentage);
	std::vector<double> vdScales;

	srand((int)time(0));
	for(unsigned int i=0; i<mvichangeIdx.size(); ++i)
	{
		int randomNum = ( rand()%((int)(maxScale*10-minScale*10)) ) + minScale*10;
		double scale = randomNum/10.0;
		//cout << scale << " ";
		vdScales.push_back(scale);
	}
	//cout << endl;

	for(int i1=0; i1<(int)mvichangeIdx.size(); ++i1)
	{
		int i2 = mvichangeIdx[i1];
		cv::Mat tmpImage, tmpSemantic;
		string strPrefix;

		double dnewCol = mvmatImages[i2].cols*vdScales[i1];
		double dnewRow = mvmatImages[i2].rows*vdScales[i1];
		cv::Size newSize = cv::Size(dnewCol, dnewRow);
		cv::resize(mvmatImages[i2], tmpImage, newSize);
		cv::resize(mvmatSemantics[i2], tmpSemantic, newSize);
		strPrefix = "RS_";

		string strTmp = mvstrPictureNames[i2];
		int pos = strTmp.find_last_of('/');
    	string strFilesName = strTmp.erase(0, pos + 1);
		string strNewRGBName = "../NewRGB/" + strPrefix + strFilesName;
		string strNewSemName = "../NewSem/" + strPrefix + strFilesName;
		cv::imwrite(strNewRGBName,tmpImage);
		cv::imwrite(strNewSemName,tmpSemantic);

		mvstrPictureNames.push_back(strNewRGBName);
		mvstrSemanticNames.push_back(strNewSemName);
		mvmatImages.push_back(tmpImage);
		mvmatSemantics.push_back(tmpSemantic);	
	}
	cout << "[INFO] Resize Finished! " << endl;
}

void Preprocessing::Tailor(const double &percentage)
{
	cout << "[INFO] Number Of Pictures Corrected For Tailor is :" ;
	Random(percentage);
	std::vector<double> vdScales;
	std::vector<double> viCoordinates;

	srand((int)time(0));
	for(unsigned int i=0; i<mvichangeIdx.size()*2; ++i)
	{
		for (unsigned int j=0; j<2; ++j){
			int randomNum = ( rand()%(int)(0.3*100) );
			viCoordinates.push_back(randomNum/100.0);

			int randomNum2rd = ( rand()%(int)(100-randomNum-0.7*100) +0.7*100);
			vdScales.push_back(randomNum2rd/100.0);
		}
	}

	for(int i1=0; i1<(int)mvichangeIdx.size(); ++i1)
	{
		int i2 = mvichangeIdx[i1];
		cv::Mat tmpImage, tmpSemantic;
		int iRow = mvmatImages[i2].rows;
		int iCol = mvmatImages[i2].cols;
		string strPrefix;
		int ix = viCoordinates[i1*2]*iCol;
		int iy = viCoordinates[i1*2+1]*iRow;
		int iw = vdScales[i1*2]*iCol;
		int ih = vdScales[i1*2+1]*iRow;
		cv::Rect ROI = cv::Rect( ix, iy, iw, ih);
		cv::Mat srcRGB = mvmatImages[i2].clone();
		cv::Mat srcSem = mvmatSemantics[i2].clone();
		srcRGB(ROI).copyTo(tmpImage);
		srcSem(ROI).copyTo(tmpSemantic);
		strPrefix = "T_";

		string strTmp = mvstrPictureNames[i2];
		int pos = strTmp.find_last_of('/');
    	string strFilesName = strTmp.erase(0, pos + 1);
		string strNewRGBName = "../NewRGB/" + strPrefix + strFilesName;
		string strNewSemName = "../NewSem/" + strPrefix + strFilesName;
		cv::imwrite(strNewRGBName,tmpImage);
		cv::imwrite(strNewSemName,tmpSemantic);

		mvstrPictureNames.push_back(strNewRGBName);
		mvstrSemanticNames.push_back(strNewSemName);
		mvmatImages.push_back(tmpImage);
		mvmatSemantics.push_back(tmpSemantic);	
	}
	cout << "[INFO] Tailor Finished! " << endl;
}

void Preprocessing::IlluminationAndContrast(const double &percentage)
{
	cout << "[INFO] Number Of Pictures Corrected For IlluminationAndContrast is :" ;
	Random(percentage);

	for(int i1=0; i1<(int)mvichangeIdx.size(); ++i1)
	{
		int i2 = mvichangeIdx[i1];
		string strPrefix;
		cv::Mat srcRGB = mvmatImages[i2].clone();
		cv::Mat srcSem = mvmatSemantics[i2].clone();
		cv::Mat tmpImage = cv::Mat::zeros( srcRGB.size(), srcRGB.type() );
		
		double alpha=1.0; /**< 控制对比度 */
		int beta=0;  /**< 控制亮度 */
		srand((int)time(0));
		for(unsigned int i=0; i<2; ++i){
			if (i==0){
				int randomNum = ( rand()%30 );
				alpha = randomNum/10.0;
			}else{
				beta = ( rand()%70 );
			}
		}

		if (i1%3==0){
			strPrefix = "IC_";
		}else if(i1%3==1){
			beta = 0;
			strPrefix = "I_";
		}else{
			alpha=1.0;
        	strPrefix = "C_";
		}

		for( int y = 0; y < srcRGB.rows; y++ )
        	for( int x = 0; x < srcRGB.cols; x++ )
            	for( int c = 0; c < 3; c++ )
                	tmpImage.at<cv::Vec3b>(y,x)[c] = cv::saturate_cast<uchar>( alpha*( srcRGB.at<cv::Vec3b>(y,x)[c] ) + beta );

		string strTmp = mvstrPictureNames[i2];
		int pos = strTmp.find_last_of('/');
    	string strFilesName = strTmp.erase(0, pos + 1);
		string strNewRGBName = "../NewRGB/" + strPrefix + strFilesName;
		string strNewSemName = "../NewSem/" + strPrefix + strFilesName;
		cv::imwrite(strNewRGBName,tmpImage);
		cv::imwrite(strNewSemName,srcSem);

		mvstrPictureNames.push_back(strNewRGBName);
		mvstrSemanticNames.push_back(strNewSemName);
		mvmatImages.push_back(tmpImage);
		mvmatSemantics.push_back(srcSem);	
	}
	cout << "[INFO] IlluminationAndContrast Finished! " << endl;
}

void Preprocessing::Saturation(const double &percentage)
{
	cout << "[INFO] Number Of Pictures Corrected For Saturation is :" ;
	Random(percentage);
	std::vector<float> vdScales;

	srand((int)time(0));
	for(unsigned int i=0; i<mvichangeIdx.size(); ++i)
	{
		int randomNum = ( rand()%((int)(100+100)) ) + (-100);
		double scale = randomNum/100.0;
		//cout << scale << " ";
		vdScales.push_back(scale);
	}

	for(int i1=0; i1<(int)mvichangeIdx.size(); ++i1)
	{
		int i2 = mvichangeIdx[i1];
		cv::Mat srcBGR;
		cv::Mat tmpImage = mvmatImages[i2].clone();
		string strPrefix;
		// https://blog.csdn.net/xingyanxiao/article/details/48035537
		mvmatImages[i2].convertTo( srcBGR,CV_32FC3);
		for(int i=0; i<srcBGR.rows; ++i)
    	{
        	for (int j=0; j<srcBGR.cols; ++j)
        	{
        		float r = srcBGR.at<cv::Vec3f>(i,j)[2];
        		float g = srcBGR.at<cv::Vec3f>(i,j)[1];
        		float b = srcBGR.at<cv::Vec3f>(i,j)[0];
        		float rgbMax = std::max(std::max(r,g),b);
        		float rgbMin = std::min(std::min(r,g),b);
        		float delta = (rgbMax-rgbMin)/255.0;
        		if (delta==0) continue;
        		float value = (rgbMax+rgbMin)/255.0;
        		float L = value/2.0;
        		float S;
        		if (L<0.5) S = delta/value;
        		else S = delta/(2.0-value);
        		float alpha, newR, newG, newB;
        		if (vdScales[i1] >=0){
        			if (vdScales[i1]+S>=1) {
        				alpha = S;
        			}else {
        				alpha = 1.0-vdScales[i1];
        			}
        			alpha = 1.0/alpha-1.0;
        			newR = r + (r-L*255.0)*alpha;
        			newG = g + (g-L*255.0)*alpha;
        			newB = b + (b-L*255.0)*alpha;
        		}else{
        			alpha = vdScales[i1];
        			newR = L*255.0 + (r-L*255.0)*(1+alpha);
        			newG = L*255.0 + (g-L*255.0)*(1+alpha); 
        			newB = L*255.0 + (b-L*255.0)*(1+alpha); 
        		}
        		//cout << newR << endl;
        		srcBGR.at<cv::Vec3f>(i,j)[2] = newR;
        		srcBGR.at<cv::Vec3f>(i,j)[1] = newG;
        		srcBGR.at<cv::Vec3f>(i,j)[0] = newB;
        	}
    	}
    	srcBGR.convertTo( tmpImage,CV_8UC3);
    	/*cv::imshow("r", mvmatImages[i2]);
    	cv::imshow("n", tmpImage);
		cv::waitKey(0);*/
		strPrefix = "S_";

		string strTmp = mvstrPictureNames[i2];
		int pos = strTmp.find_last_of('/');
    	string strFilesName = strTmp.erase(0, pos + 1);
		string strNewRGBName = "../NewRGB/" + strPrefix + strFilesName;
		string strNewSemName = "../NewSem/" + strPrefix + strFilesName;
		cv::imwrite(strNewRGBName,tmpImage);
		cv::imwrite(strNewSemName,mvmatSemantics[i2]);

		mvstrPictureNames.push_back(strNewRGBName);
		mvstrSemanticNames.push_back(strNewSemName);
		mvmatImages.push_back(tmpImage);
		mvmatSemantics.push_back(mvmatSemantics[i2]);	
	}
	cout << "[INFO] Saturation Finished! " << endl;
}


void Preprocessing::Mosaic(const double &percentage)
{
	cout << "[INFO] Number Of Pictures Corrected For Mosaic is :" ;
	Random(percentage);
	std::vector<int> viIdx4Mosaic;
	std::vector<double> vdCor4Mosaic;

	srand((int)time(0));
	for(unsigned int i=0; i<mvichangeIdx.size()*4; ++i)
	{
		int randomNum1 = ( rand()%((int)(mvichangeIdx.size())) );
		viIdx4Mosaic.push_back(randomNum1);
		if (i%4==0){
			for (unsigned int j=0; j<2; ++j){
				int randomNumj = ( rand()%((int)(0.7*100-0.3*100))+0.3*100 );
				double scalej = randomNumj/100.0;
				vdCor4Mosaic.push_back(scalej);
			}
		}
	}

	for(int i1=0; i1<(int)mvichangeIdx.size(); ++i1)
	{
		int tmpCol = mvmatImages[ viIdx4Mosaic[i1*4] ].cols;
		int tmpRow = mvmatImages[ viIdx4Mosaic[i1*4] ].rows;
		std::vector<cv::Mat> vmatRSRGBList,vmatTRGBList;
		std::vector<cv::Mat> vmatRSSemList,vmatTSemList;
		cv::Mat tmpMergeImage, tmpMergeSemantic;
		vmatRSRGBList.resize(4);
		vmatRSSemList.resize(4);
		for (unsigned int i2=0; i2<4; ++i2)
		{
			cv::Size newSize = cv::Size(tmpCol, tmpRow);
			cv::resize(mvmatImages[ viIdx4Mosaic[i1*4+i2] ], vmatRSRGBList[i2], newSize);
			cv::resize(mvmatSemantics[ viIdx4Mosaic[i1*4+i2] ], vmatRSSemList[i2], newSize);
		}
		int width = vdCor4Mosaic[i1*2+0]*tmpCol;
		int height = vdCor4Mosaic[i1*2+1]*tmpRow;
		cv::Rect rect1 = cv::Rect( 0, 0, width, height );
		cv::Rect rect2 = cv::Rect( tmpCol-width, 0, width, height );
		cv::Rect rect3 = cv::Rect( 0, tmpRow-height, width, height );
		cv::Rect rect4 = cv::Rect( tmpCol-width, tmpRow-height, width, height );
		cv::Rect rectList[4] = { rect1, rect2, rect3, rect4 };
		for (unsigned int i2=0; i2<4; ++i2)
		{
			cv::Mat tmpTailorRGB, tmpTailorSem;
			vmatRSRGBList[i2](rectList[i2]).copyTo(tmpTailorRGB);
			vmatRSSemList[i2](rectList[i2]).copyTo(tmpTailorSem);
			vmatTRGBList.push_back(tmpTailorRGB);
			vmatTSemList.push_back(tmpTailorSem);
		}
		cv::Mat tmpRGBRow1st,tmpRowRGB2rd, tmpSemRow1st,tmpSemRow2rd;
		hconcat(vmatTRGBList[0],vmatTRGBList[1],tmpRGBRow1st);
		hconcat(vmatTRGBList[2],vmatTRGBList[3],tmpRowRGB2rd);
		vconcat(tmpRGBRow1st,tmpRowRGB2rd,tmpMergeImage);
		// tmpMergeSemantic
		hconcat(vmatTSemList[0],vmatTSemList[1],tmpSemRow1st);
		hconcat(vmatTSemList[2],vmatTSemList[3],tmpSemRow2rd);
		vconcat(tmpSemRow1st,tmpSemRow2rd,tmpMergeSemantic);
		string strPrefix = "M4_";

		string strTmp = mvstrPictureNames[ viIdx4Mosaic[i1*4] ];
		int pos = strTmp.find_last_of('/');
    	string strFilesName = strTmp.erase(0, pos + 1);
		string strNewRGBName = "../NewRGB/" + strPrefix + strFilesName;
		string strNewSemName = "../NewSem/" + strPrefix + strFilesName;
		cv::imwrite(strNewRGBName,tmpMergeImage);
		cv::imwrite(strNewSemName,tmpMergeSemantic);
	}
	cout << "[INFO] Mosaic Finished! " << endl;
}

void Preprocessing::RandomErasure(const double &percentage)
{
	cout << "[INFO] Number Of Pictures Corrected For RandomErasure is :" ;
	Random(percentage);
	std::vector<int> viGridNum;
	std::vector<int> viGridIdx;

	srand((int)time(0));
	for(unsigned int i=0; i<mvichangeIdx.size(); ++i)
	{
		int randomNum =  rand()%(6 -3) + 3;
		viGridNum.push_back(randomNum);
		int randomNum2rd =  rand()%(int)( pow(randomNum,2) );
		viGridIdx.push_back(randomNum2rd);
	}

	for(int i1=0; i1<(int)mvichangeIdx.size(); ++i1)
	{
		int i2 = mvichangeIdx[i1];
		cv::Mat tmpImage = mvmatImages[i2].clone();
		cv::Mat tmpSemantic = mvmatSemantics[i2].clone();
		string strPrefix;
		int iGridWidth = tmpImage.cols/viGridNum[i1];
		int iGridHeight = tmpImage.rows/viGridNum[i1];
		double dx = viGridIdx[i1]%viGridNum[i1]*iGridWidth;
		double dy = viGridIdx[i1]/viGridNum[i1]*iGridHeight;

		//cv::Mat tmpRGBMask = cv::Mat::zeros( tmpImage.size(), tmpImage.type() );
		cv::Mat tmpMask = cv::Mat::zeros( tmpSemantic.size(), CV_8UC1 );
		//cv::Mat Mask = tmpMask( cv::Rect(dx, dy, iGridWidth, iGridHeight) );
		tmpMask( cv::Rect(dx, dy, iGridWidth, iGridHeight) ).setTo(255);
		//tmpMask.setTo(0, tmpMask);
    	tmpImage.setTo(0, tmpMask);
    	tmpSemantic.setTo(0, tmpMask);
		
		strPrefix = "RE_";

		string strTmp = mvstrPictureNames[i2];
		int pos = strTmp.find_last_of('/');
    	string strFilesName = strTmp.erase(0, pos + 1);
		string strNewRGBName = "../NewRGB/" + strPrefix + strFilesName;
		string strNewSemName = "../NewSem/" + strPrefix + strFilesName;
		cv::imwrite(strNewRGBName,tmpImage);
		cv::imwrite(strNewSemName,tmpSemantic);
	}
	cout << "[INFO] RandomErasure Finished! " << endl;
}
