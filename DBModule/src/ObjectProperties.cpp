/*******************************************************************************
 *
 *  ObjectProperties.cpp
 *
 *******************************************************************************/

#include "DBModule/include/ObjectProperties.hpp"

ObjectProperties::ObjectProperties( std::string name )
                  :m_Name(name),m_Identifier("1")
{	
   FeatureExtractor fextractor(name);
   fextractor.extractKeyPoint(featureList,Descriptor);
   height = fextractor.getImgHeight();
   width = fextractor.getImgWidth();
   feature_count = featureList.size();
}

// ObjectProperties::ObjectProperties()
// {   
// }

ObjectProperties::~ObjectProperties()
{    
}

ObjectProperties::ObjectProperties(const ObjectProperties& other)
{
    *this = other;
}

ObjectProperties& ObjectProperties::operator=(const ObjectProperties& other)
{    
	featureList.clear();
    m_Name = other.m_Name;
    for ( unsigned i=0; i < other.featureList.size(); i++)
    {
		featureList.push_back( other.featureList[i]);
    }
    return *this;
}

void ObjectProperties::printOn( std::ostream& strm )
{
    strm << "Object Name: " << m_Name << std::endl;
    strm << "Object Iden: " << m_Identifier << std::endl << std::endl;
	strm << "Object Height: " << height << std::endl;
	strm << "Object Width: " << width << std::endl << std::endl;
	strm << "Number keypoints in images: " << feature_count << std::endl;
    strm << std::endl << std::endl;
}

std::string ObjectProperties::getName(){
    return m_Name;
}

int ObjectProperties::getHeight(){
    return height;
}

int ObjectProperties::getwidth(){
    return width;
}

std::vector<cv::KeyPoint> ObjectProperties::getKPlist(){
    return featureList;
}

cv::Mat  ObjectProperties::getDescriptor(){
    return Descriptor;
}


// void ObjectProperties::featchObjectProperties() {
// 	cv::Ptr<cv::FeatureDetector> detector = cv::BRISK::create();
// 	cv::Ptr<cv::DescriptorExtractor> descriptorExtractor = cv::BRISK::create();
// //	KeyPointExtractor* m_Extractor;
// //	m_Extractor = DefaultExtractor::createInstance();
// //	std::cout << " Selected feature extractor: " << m_Extractor->getName() << std::endl << std::endl << m_Extractor->getDescription() << std::endl;
// 	cv::Mat Img = cv::imread(m_Name,cv::IMREAD_GRAYSCALE);
// 	if ( Img.empty() ) {
// 		 std::cout << "Empty Image " << std::endl;
// 		 return ;
// 	}
// 	height = Img.rows;
// 	width = Img.cols;
// 	detector->detect(Img, featureList);
// 	descriptorExtractor->compute(Img, featureList, Descriptor);
// 	feature_count = featureList.size();
// //	m_Extractor->setImage(Img);
// //	m_Extractor->getCVKeyPoints(featureList,Descriptot);
//     //FeatureExtractor fextractor(ImageFileName);
// 	//fextractor.extractKeyPoint(keyPointList,Descriptor);
	
// 	std::cout << "Feature extraction Done " << std::endl;
// }

// void ObjectProperties::testLoadedObjects() {
// 	cv::Mat Img = cv::imread(m_Name, cv::IMREAD_UNCHANGED);
// 	if (Img.empty()) {
// 		LOGEROR("Images Files is not Valid");
// 	}
// 	cv::Mat Display;
// 	cv::drawKeypoints(Img, featureList, Display, cv::Scalar(0, 0, 255), 2);
// 	boost::filesystem::path p(m_Name);
// 	std::string FileName = "TestArchive_" + std::to_string(feature_count)+".jpg";
// 	boost::filesystem::path  newPath(p.parent_path().stem().string()+ FileName);
// 	LOGINFO("Writing Test Images " + newPath.string());
// 	cv::imwrite(FileName, Display);
// }