#ifndef FEATUREMATCHER_H
#define FEATUREMATCHER_H

//#include "ExtractFeature.hpp"
//#include "Polygon.hpp"
#include <set>
#include <iostream>
#include <opencv2/stitching/detail/matchers.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include "DBModule/include/DBModule.hpp"
#include "FeatureExtraction/include/FeatureExtractor.hpp"

class FeatureMatcher: public DBModule ,public ObjectProperties ,public FeatureExtractor
{
private:
	static const double FEATURE_RATIO_TEST_THRESHOLD;
	static const int    MIN_NUM_MATCHES;
	static const int    HOMOGRAPHY_MODEL_MIN_POINTS;
	static const double MAX_INLIERS_DIST;
	static const double INLIER_PROBABILITY;
	static const double OPENCV_DEFAULT_CONFIDENCE;
	static const double INLIER_TOLERANT_STD_DISTANCE;
	static const double HOMO_MAX_PERSPECTIVE;
	static const double EPSILON;
	static const double INLIER_IN_MATCH_RATIO;
	static const double CONFIDENCE_THRESHOLD;
	bool  SHOW_INTERMEDIATE_RESULT;
	std::string selfImagePath;
	cv::Mat selfImage;
	int ImageHeight,ImageWidth;
	std::vector<cv::KeyPoint> self_keyPointList;
	cv::Mat self_Descriptor;
	
	bool getInitialMatches(cv::detail::MatchesInfo& _M_info, 
	                       const ObjectProperties* _productImageinfo) ;
	bool SecondLevelMatchs(cv::detail::MatchesInfo & _M_info,
	                       cv::Mat &_H,
						   const std::vector<cv::KeyPoint> &_prodKeyPointList);
	bool ValidteHomoGraphy(const cv::detail::MatchesInfo &M_info) ;
	std::vector<cv::Point2f> estimateOverlapRegion(const cv::detail::MatchesInfo &_M_info, const std::vector<cv::detail::ImageFeatures> &_features);
	bool calculateConfidence(cv::detail::MatchesInfo &M_info,const std::vector<cv::detail::ImageFeatures> &_features);
	bool estimateHomography(cv::detail::MatchesInfo &_M_info,const std::vector<cv::detail::ImageFeatures> &_features);
	void drawMatche(const cv::detail::MatchesInfo &_M_info,
	                const ObjectProperties* _productImageinfo);
	void drawcorners(const cv::detail::MatchesInfo &_pairwise_matches,const std::vector<cv::Point2f> &_overlap_boundary,
		             const std::vector<cv::Mat> &_images);
public:
	FeatureMatcher(const std::string &_selfImageName);
	bool BuildMatches(std::vector<cv::detail::MatchesInfo> &_MatchesPairList);
	void displayMatchesInfo(std::vector<cv::detail::MatchesInfo> &pairwise_matches);
};

#endif