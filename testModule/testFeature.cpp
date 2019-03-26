#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include "ProjectProperty/include/Config.hpp"
#include "ProjectProperty/include/ProjectProperty.hpp"
#include "FeatureExtraction/include/FeatureExtractor.hpp"

int main() {
	LOGINFO("INSIDE MIAN MODULE ");
	//const std::string DirectoryName = "D:/C++_Developments/Object_detection/Config";
	Config::getConfigFile(Config_directory);
	//std::vector<std::string> profilesToLoad;
	//Config::LoadProfiles(DirectoryName, profilesToLoad);
	Config::LoadProfiles();
	//std::cout << "Total Number of Property Loaded " << Config::profilesToLoad.size() << std::endl;
	//Config::loadConfig(DirectoryName, profilesToLoad);
	Config::loadConfig();
    std::string ImageFileName="D:/Python/OCR/input/Image/50Test_distinct_scn/20180627_095634.jpg";
    std::vector<cv::KeyPoint> keyPointList;	
	cv::Mat Descriptor;
    FeatureExtractor fextractor(ImageFileName);
	fextractor.extractKeyPoint(keyPointList,Descriptor);
	fextractor.DisplayKeyPoints(keyPointList);
	return 0;
}