#include "ProjectProperty/include/Config.hpp"
#include "ProjectProperty/include/ProjectProperty.hpp"
#include <iostream>

int main() {
	//const std::string DirectoryName = "D:/C++_Developments/Object_detection/Config";
	Config::getConfigFile(Config_directory);
	//std::vector<std::string> profilesToLoad;
	//Config::LoadProfiles(DirectoryName, profilesToLoad);
	Config::LoadProfiles();
	std::cout << "Total Number of Property Loaded " << Config::profilesToLoad.size() << std::endl;
	//Config::loadConfig(DirectoryName, profilesToLoad);
	Config::loadConfig();
	int a = Config::getInt("ObjectRecognition.Stage1Matcher");
	float b = Config::getFloat("NNRMatching.MaxNearestNeighbourRatio");
	bool c = Config::getBool("HoughClustering.Plot");
	std::string d = Config::getString("Path.DBobjPath");
	std::cout << a <<" , "<< b <<" , "<<c <<","<< d <<std::endl;
	return 0;
}