#include <opencv2/stitching/detail/matchers.hpp>
#include "FeatureMatch/include/FeatureMatcher.hpp"

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
    /* std::string ImageFileName="D:/Python/OCR/input/Image/50Test_distinct_scn/20180627_095634.jpg";
    FeatureMatch fmatcher(ImageFileName);
    std::vector<cv::detail::MatchesInfo> MatchesPairList;
    fmatcher.BuildMatches(MatchesPairList); */
	return 0;
}