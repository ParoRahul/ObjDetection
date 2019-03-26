#include "FeatureMatch/include/FeatureMatcher.hpp"

const double FeatureMatcher::FEATURE_RATIO_TEST_THRESHOLD = 0.75;
const int    FeatureMatcher::MIN_NUM_MATCHES = 6;
const int    FeatureMatcher::HOMOGRAPHY_MODEL_MIN_POINTS = 4;
const double FeatureMatcher::MAX_INLIERS_DIST = 5.0;
const double FeatureMatcher::INLIER_PROBABILITY = 0.225;
const double FeatureMatcher::OPENCV_DEFAULT_CONFIDENCE = 0.995;
const double FeatureMatcher::INLIER_TOLERANT_STD_DISTANCE = 4.25;
const double FeatureMatcher::HOMO_MAX_PERSPECTIVE = 2e-3;
const double FeatureMatcher::EPSILON = 1e-6;
const double FeatureMatcher::INLIER_IN_MATCH_RATIO = 0.1;
const double FeatureMatcher::CONFIDENCE_THRESHOLD = 0.01;
	

FeatureMatcher::FeatureMatcher(const std::string &_selfImageName)
:selfImagePath(_selfImageName),SHOW_INTERMEDIATE_RESULT(true),DBModule("LOAD"),
 FeatureExtractor (_selfImageName)
{
	
    FeatureExtractor::extractKeyPoint(self_keyPointList,self_Descriptor);
    ImageHeight = FeatureExtractor::getImgHeight();
    ImageWidth = FeatureExtractor::getImgWidth();
	if (SHOW_INTERMEDIATE_RESULT)
	   selfImage=cv::imread(_selfImageName,cv::IMREAD_UNCHANGED);
}

bool FeatureMatcher::BuildMatches(std::vector<cv::detail::MatchesInfo> &_MatchesPairList ) 
{
try {
	int objectCounter=0;
	for (std::map<std::string, std::string>::iterator it = DBModule::ImageListDBMap.begin(); 
	     it != ImageListDBMap.end(); ++it)
	{
		if (it->second.empty()) {
			continue;
		}
		objectCounter++;
		ObjectProperties* productImageinfo =new ObjectProperties();
		productImageinfo = DBModule::loadDBObjects(it->second);
		cv::detail::MatchesInfo MatchInformation;
		MatchInformation.src_img_idx = 0;
		MatchInformation.dst_img_idx = objectCounter;
		MatchInformation.matches.clear();
		MatchInformation.inliers_mask.clear();
		MatchInformation.num_inliers = 0;
		MatchInformation.confidence = 0.0;
		cv::Mat H;
		LOGINFO ("\nProcessing Image  : " + productImageinfo->getName());
		if (!getInitialMatches(MatchInformation,productImageinfo->getDescriptor()))
			continue;
		if (SHOW_INTERMEDIATE_RESULT) 
		    drawMatche(MatchInformation,productImageinfo);
		LOGINFO (" Number Of matches After Stage 1 is " );
		// if (!SecondLevelMatchs(MatchInformation,H,_features))
		// 	continue;
		// if (SHOW_INTERMEDIATE_RESULT) 
		// drawMatche(MatchInformation,productImageinfo);
		// LOGINFO (" Number Of matches After Stage 2 is : " );
		// if (!ValidteHomoGraphy(_MatchesPairList[PairIndx])) {
		// 	LOGINFO (" Inavlid Homography" );
		// 	continue;
		// }
		// if (!calculateConfidence(_MatchesPairList[PairIndx], _features)) {
		// 	LOGINFO(" calculateConfidence calculation fail " );
		// 	continue;
		// }
		// if (!estimateHomography(_MatchesPairList[PairIndx], _features)) {
		// 	LOGINFO (" estimateHomography calculation fail ");
		// 	continue;
		// }
		delete productImageinfo;
		_MatchesPairList.push_back(MatchInformation);
	}
	displayMatchesInfo(_MatchesPairList);
}
catch (std::exception e) {
	LOGEROR ("[ERROR] FeatureMatcher " );
	throw;
}
catch (...) {
	LOGEROR ("[ERROR] FeatureMatcher Unexpected Exception Occured" );
	throw;
}
return true;
}

bool FeatureMatcher::getInitialMatches(cv::detail::MatchesInfo& _M_info,
                                        const ObjectProperties* _productImageinfo)
{
	auto getMean = [&](const std::vector<cv::DMatch> &Dlist) {
		if (Dlist.size() < 0) {
			return 0.0;
		}
		double mean = 0, sum = 0;
		int numberOfMember = 0;
		for (cv::DMatch D : Dlist) {
			sum += D.distance;
			numberOfMember++;
		}
		mean = sum / numberOfMember;
		return mean;
	};
	auto getStandardDeviation = [&](std::vector<cv::DMatch> &Dlist, const double mean) {
		if (Dlist.size() < 0) {
			return 0.0;
		}
		double stddev = 0, sum = 0;
		int numberOfMember = 0;
		for (cv::DMatch D : Dlist) {
			sum += pow((D.distance - mean), 2);
			numberOfMember++;
		}
		stddev = sqrt(sum / (numberOfMember));
		return stddev;
	};
	cv::Ptr<cv::DescriptorMatcher> descriptorMatcher = DescriptorMatcher::create(cv::NORM_L2);
	std::vector <std::vector<cv::DMatch>> DMatchList;
	std::vector <cv::DMatch> MatchesList;
	descriptorMatcher->knnMatch(self_Descriptor, _productImageinfo->getDescriptor(), DMatchList, 2);
	for (size_t i = 0; i < DMatchList.size(); i++) {
		 if (DMatchList[i].size() < 2)
			continue;
		const cv::DMatch& m0 = DMatchList[i][0];
		const cv::DMatch& m1 = DMatchList[i][1];
		if (m0.distance < FEATURE_RATIO_TEST_THRESHOLD * m1.distance)
			MatchesList.push_back(m0);
		else if ((m1.distance < FEATURE_RATIO_TEST_THRESHOLD *m0.distance))
			MatchesList.push_back(m1);
		else
			continue;
	}
	double mean = getMean(MatchesList), stddev = getStandardDeviation(MatchesList, mean);
	double D_THRESHOLD = (INLIER_TOLERANT_STD_DISTANCE * stddev) + mean;
	LOGINFO (" Distance Threshold :"+std::stod(D_THRESHOLD));
	for (cv::DMatch D : MatchesList) {
		if (D.distance <= D_THRESHOLD)
			_M_info.matches.push_back(D);
	}
	double Match2KP = 2.0 * _M_info.matches.size() / (self_keyPointList.size() + _productImageinfo->getKPlist().size());
	LOGINFO (" Kp to Match Ratio "+std::stod(Match2KP));
	return true;
}

// bool FeatureMatcher::SecondLevelMatchs(cv::detail::MatchesInfo &_M_info, 
//                                        cv::Mat &_H,
// 									   const std::vector<cv::KeyPoint> &_prodKeyPointList)
// {
// 	if (_M_info.matches.size() < MIN_NUM_MATCHES)
// 		return false;
// 	std::vector<cv::Point2f> src_points1;
// 	std::vector<cv::Point2f> dst_points1;
// 	std::vector<cv::Point2f> src_points2;
// 	std::vector<cv::Point2f> dst_points2;
// 	for (size_t i = 0; i < _M_info.matches.size(); i++)
// 	{
// 		const DMatch& m = _M_info.matches[i];
// 		Point2f p1 = self_keyPointList[m.queryIdx].pt;
// 		src_points1.push_back(p1);
// 		Point2f p2 = _prodKeyPointList[m.trainIdx].pt;
// 		dst_points1.push_back(p2);

// 		Point2f p3 = self_keyPointList[m.queryIdx].pt;
// 		p3.x -= _features[_M_info.src_img_idx].img_size.width * 0.5f;
// 		p3.y -= _features[_M_info.src_img_idx].img_size.height * 0.5f;
// 		src_points2.push_back(p3);
// 		Point2f p4 = _prodKeyPointList[m.trainIdx].pt;
// 		p4.x -= _features[_M_info.dst_img_idx].img_size.width * 0.5f;
// 		p4.y -= _features[_M_info.dst_img_idx].img_size.height * 0.5f;
// 		dst_points2.push_back(p4);
// 	}
// 	vector<DMatch> dMatchList;
// 	_M_info.inliers_mask.clear();
// 	vector<char> mask;
// 	const int MAX_ITERATION = (int)(log(1 - OPENCV_DEFAULT_CONFIDENCE) / log(1 - pow(INLIER_PROBABILITY, HOMOGRAPHY_MODEL_MIN_POINTS)));
// 	_M_info.H = findHomography(src_points1, dst_points1,CV_RANSAC, MAX_INLIERS_DIST, _M_info.inliers_mask, MAX_ITERATION, OPENCV_DEFAULT_CONFIDENCE);
// 	_H = findHomography(src_points1, dst_points1,CV_RANSAC, MAX_INLIERS_DIST, mask, MAX_ITERATION, OPENCV_DEFAULT_CONFIDENCE);
// 	for (int i = 0; i < _M_info.inliers_mask.size(); ++i) {
// 		if (_M_info.inliers_mask[i]) {
// 			_M_info.num_inliers++;
// 			dMatchList.push_back(_M_info.matches[i]);
// 		}
// 	}
// 	if (_M_info.num_inliers < MIN_NUM_MATCHES) {
// 		_M_info.confidence = 0;
// 		return false;
// 	}
// 	else {
// 		_M_info.confidence = _M_info.num_inliers / (8 + 0.3 * _M_info.matches.size());
// 		_M_info.matches.clear();
// 		_M_info.matches = dMatchList;
// 		//drawMatche(_M_info,_features);
// 	}
// 	return true;
// }

// bool FeatureMatcher::estimateHomography(MatchesInfo &_M_info,const std::vector<cv::detail::ImageFeatures> &_features) {
// 	if (_M_info.matches.size() < MIN_NUM_MATCHES)
// 		return false;
// 	const ImageFeatures &Features1 = _features[_M_info.src_img_idx];
// 	const ImageFeatures &Features2 = _features[_M_info.dst_img_idx];
// 	vector<Point2f> src_points;
// 	vector<Point2f> dst_points;
// 	for (size_t i = 0; i < _M_info.matches.size(); i++)
// 	{
// 		const DMatch& m = _M_info.matches[i];
// 		Point2f p1 = Features1.keypoints[m.queryIdx].pt;
// 		p1.x -= Features1.img_size.width * 0.5f;
// 		p1.y -= Features1.img_size.height * 0.5f;
// 		src_points.push_back(p1);
// 		Point2f p2 = Features2.keypoints[m.trainIdx].pt;
// 		p2.x -= Features2.img_size.width * 0.5f;
// 		p2.y -= Features2.img_size.height * 0.5f;
// 		dst_points.push_back(p2);
// 	}
// 	vector<DMatch> dMatchList;
// 	vector<char> mask;
// 	double inlier_probability = 0.2;
// 	const int MAX_ITERATION = (int)(log(1 - OPENCV_DEFAULT_CONFIDENCE) / log(1 - pow(inlier_probability, HOMOGRAPHY_MODEL_MIN_POINTS)));
// 	_M_info.H = findHomography(src_points,dst_points,CV_RANSAC,MAX_INLIERS_DIST,mask,MAX_ITERATION,OPENCV_DEFAULT_CONFIDENCE);
// 	//_M_info.H.convertTo(_M_info.H, CV_64FC1);
// 	return true;
// }

// bool FeatureMatcher::calculateConfidence(MatchesInfo &_M_info,const std::vector<cv::detail::ImageFeatures> &_features) {
// 	const ImageFeatures &Features1 = _features[_M_info.src_img_idx];
// 	const ImageFeatures &Features2 = _features[_M_info.dst_img_idx];
// 	auto get_match_cnt = [&](vector<Point2f>& poly, bool first) {
// 		if (poly.size() < 3) return 0;
// 		polygon OverlapBoundary(poly);
// 		int cnt = 0;
// 		for (auto& m : _M_info.matches) {
// 			Point2f p = first ? Features2.keypoints[m.trainIdx].pt : Features1.keypoints[m.queryIdx].pt;
// 			if (OverlapBoundary.insidePolygon(p))
// 				cnt++;
// 		}
// 		return cnt;
// 	};

// 	auto get_keypoint_cnt = [&](vector<Point2f>& poly, bool first) {
// 		if (poly.size() < 3) return 0;
// 		polygon OverlapBoundary(poly);
// 		int cnt = 0;
// 		for (auto& p : first ? Features2.keypoints : Features1.keypoints) {
// 			if (OverlapBoundary.insidePolygon(p.pt))
// 				cnt++;
// 		}
// 		return cnt;
// 	};

// 	vector<Point2f> overlap12 = estimateOverlapRegion(_M_info,_features);
// 	if (overlap12.empty()) {
// 		cout << " OverLap Region [overlap12] is empty" << endl;
// 		return false;
// 	}
// 	//drawcorners(_M_info,overlap12);
// 	cout << " Match Count " << get_match_cnt(overlap12, true) << " get_keypoint_cnt " << get_keypoint_cnt(overlap12, true) << endl;
// 	float inlier2match_ratio1 = _M_info.num_inliers * 1.0f / get_match_cnt(overlap12, true);
// 	if (inlier2match_ratio1 < INLIER_IN_MATCH_RATIO) {
// 		cout << " Inlier to match Ratio[r1m] is bellow Threshold" << endl;
// 		return false;
// 	}

// 	float inlier2Kpoint_ratio1 = _M_info.num_inliers* 1.0f / get_keypoint_cnt(overlap12, true);
// 	if (inlier2Kpoint_ratio1 < 0.01 || inlier2Kpoint_ratio1 > 1) {
// 		cout << " r2p Is not in Range " << endl;
// 		return false;
// 	}

// 	MatchesInfo M_infoInv;
// 	M_infoInv.src_img_idx = _M_info.dst_img_idx;
// 	M_infoInv.dst_img_idx = _M_info.src_img_idx;
// 	M_infoInv.H = _M_info.H.inv();

// 	vector<Point2f> overlap21 = estimateOverlapRegion(M_infoInv,_features);
// 	if (overlap21.empty()) {
// 		cout << " OverLap Region [overlap21] is empty" << endl;
// 		return false;
// 	}

// 	cout << " Match Count " << get_match_cnt(overlap21, false) << " get_keypoint_cnt " << get_keypoint_cnt(overlap21, false) << endl;
// 	float inlier2match_ratio2 = _M_info.num_inliers * 1.0f / get_match_cnt(overlap21, false);
// 	if (inlier2match_ratio2 < INLIER_IN_MATCH_RATIO) {
// 		cout << " Inlier to match Ratio[r2m] is bellow Threshold" << endl;
// 		return false;
// 	}

// 	float inlier2Kpoint_ratio2 = _M_info.num_inliers* 1.0f / get_keypoint_cnt(overlap21, false);
// 	if (inlier2Kpoint_ratio2 < 0.01 || inlier2Kpoint_ratio2 > 1) {
// 		cout << " r2p Is not in Range " << endl;
// 		return false;
// 	}
	
// 	_M_info.confidence = 0.5*(inlier2Kpoint_ratio1 + inlier2Kpoint_ratio2);
// 	//_M_info.confidence = _M_info.num_inliers * 1.0f / (8.0f+0.3*_M_info.inliers_mask.size());
// 	if (_M_info.confidence < CONFIDENCE_THRESHOLD) {
// 		cout << " Confidence Bellow Threshold" << endl;
// 		return false;
// 	}
// 	polygon OverlapBoundary(overlap12);
// 	double overLap_area = OverlapBoundary.Calculate_area();
// 	double area = Features2.img_size.width * Features2.img_size.height;
// 	cout << " OverlapArea = " << overLap_area << " ImageArea = " << area << endl;
// 	if (overLap_area / area < 0.15) {
// 		cout << " Overlap Area Bellow Threshold" << endl;
// 		return false;
// 	}
// 	return true;
// }

void FeatureMatcher::displayMatchesInfo(vector<MatchesInfo> &pairwise_matches) {
	for (MatchesInfo M : pairwise_matches) {
		//if (M.confidence == 0 || (M.src_img_idx == M.dst_img_idx)) continue;
		std::cout << " \nPAIR              : " << M.src_img_idx << "," << M.dst_img_idx << std::endl;
		std::cout << " Number Of Matches   : " << M.inliers_mask.size() << std::endl;
		std::cout << " Number Of Inlier    : " << M.num_inliers << std::endl;
		std::cout << " Confidence          : " << M.confidence << std::endl;
		std::cout << " Homography Mat Size : " << M.H.size() << std::endl;
		std::cout << " Homography Mat Type : " << M.H.type() << std::endl;
	}
}

void FeatureMatcher::drawMatche(const MatchesInfo &_M_info,
                                const ObjectProperties* _productImageinfo) {
	//Mat Img1 = _images[_M_info.src_img_idx].clone();
	cv::Mat prodImage = cv::imread(_productImageinfo->getName(),cv::IMREAD_UNCHANGED);
	//auto kp1 = self_keyPointList;
	//auto kp2 = _productImageinfo->featureList;
	cv::Mat Result;
	if (_M_info.num_inliers == 0) {
		size_t n = _M_info.matches.size();
		String fileName = OUTPUTPATH + "Matches_Before" + to_string(_M_info.src_img_idx) + "_" + to_string(_M_info.dst_img_idx) + "_" + to_string(n) + ".jpg";
		drawMatches(selfImage, self_keyPointList, prodImage,productImageinfo->getKPlist(),
		            _M_info.matches, Result, Scalar(0, 0, 255), Scalar(0, 0, 255), Mat(), 2);
		imwrite(fileName, Result);
	}
	else {
		size_t n = _M_info.matches.size();
		String fileName = OUTPUTPATH + "Matches_After" + to_string(_M_info.src_img_idx) + "_" + to_string(_M_info.dst_img_idx) + "_" + to_string(n) + ".jpg";
		drawMatches(selfImage, self_keyPointList, prodImage,productImageinfo->getKPlist(),
		            _M_info.matches, Result, Scalar(255, 0, 0), Scalar(0, 0, 255), Mat(), 2);
		imwrite(fileName, Result);
	}
}

bool FeatureMatcher::ValidteHomoGraphy(const MatchesInfo &M_info) {
	const double det = M_info.H.at<double>(0, 0) * M_info.H.at<double>(1, 1) - M_info.H.at<double>(1, 0) *M_info.H.at<double>(0, 1);
	if (det < 0)
		return false;
	const double N1 = sqrt(M_info.H.at<double>(0, 0)*M_info.H.at<double>(0, 0) + M_info.H.at<double>(1, 0)*M_info.H.at<double>(1, 0));
	if (N1 > 4 || N1 < 0.1)
		return false;
	const double N2 = sqrt(M_info.H.at<double>(0, 1) * M_info.H.at<double>(0, 1) + M_info.H.at<double>(1, 1) * M_info.H.at<double>(1, 1));
	if (N2 > 4 || N2 < 0.1)
		return false;
	const double N3 = sqrt(M_info.H.at<double>(2, 0) * M_info.H.at<double>(2, 0) + M_info.H.at<double>(2, 1) * M_info.H.at<double>(2, 1));
	if (N3 > HOMO_MAX_PERSPECTIVE)
		return false;
	if (fabs(M_info.H.at<double>(2, 0)) > HOMO_MAX_PERSPECTIVE)
		return false;
	if (fabs(M_info.H.at<double>(2, 1)) > HOMO_MAX_PERSPECTIVE)
		return false;
	return true;
}

vector<Point2f> FeatureMatcher::estimateOverlapRegion(const MatchesInfo &_M_info,const std::vector<cv::detail::ImageFeatures> &_features) {
	vector<Point2f> overLapBoundary;
	const Size &size1 = _features[_M_info.src_img_idx].img_size;
	const Size &size2 = _features[_M_info.dst_img_idx].img_size;
	const Mat  &homo = _M_info.H;
	const int NR_POINT_ON_EDGE = 200;
	float ostepw = size1.width * 1.0f / NR_POINT_ON_EDGE,
		osteph = size1.height * 1.0f / NR_POINT_ON_EDGE;
	float sstepw = size2.width * 1.0f / NR_POINT_ON_EDGE,
		ssteph = size2.height * 1.0f / NR_POINT_ON_EDGE;
	vector<Point2f> obj_corners;
	vector<Point2f> scn_corners;
	vector<Point2f> Transformed_corners;
	for (int i = 0; i < NR_POINT_ON_EDGE; i++) {
		obj_corners.push_back(Point2f(i * ostepw, 0.0f));
		obj_corners.push_back(Point2f(i * ostepw, size1.height*1.0f));
		obj_corners.push_back(Point2f(0.0f, i * osteph));
		obj_corners.push_back(Point2f(size1.width*1.0f, i*osteph));
		scn_corners.push_back(Point2f(i * sstepw, 5.0f));
		scn_corners.push_back(Point2f(i * sstepw, size2.height - 5.0f));
		scn_corners.push_back(Point2f(5.0f, i * ssteph));
		scn_corners.push_back(Point2f(size2.width - 5.0f, i * ssteph));
	}
	perspectiveTransform(obj_corners, Transformed_corners, homo);
	if (Transformed_corners.empty())
		return overLapBoundary;
	polygon TransformedBoundary(Transformed_corners);
	for (auto& p : scn_corners) {
		if (TransformedBoundary.insidePolygon(p))
			Transformed_corners.push_back(p);
	}
	vector<Point2f> pts1in2;
	Rect Imag2Rect(-5, -5, size2.width + 5, size2.height + 5);
	for (Point2f P : Transformed_corners) {
		if (Imag2Rect.contains(P))
			pts1in2.push_back(P);
	}
	if (!pts1in2.empty())
		convexHull(pts1in2, overLapBoundary, false);
	return overLapBoundary;
}

void FeatureMatcher::drawcorners(const MatchesInfo &_M_info, const vector<Point2f> &_overlap_boundary, const std::vector<cv::Mat> &_images) {
	int count = 0;
	Mat Imageto = _images[_M_info.dst_img_idx].clone();
	for (Point P : _overlap_boundary) {
		circle(Imageto, P, 2, Scalar(0, 0, 255), 2, 0);
		count++;
	}
	String fileName = OUTPUTPATH + "BOUNDARY_" + to_string(_M_info.src_img_idx) + "_" + to_string(_M_info.dst_img_idx) + "_" + to_string(count) + ".jpg";
	imwrite(fileName, Imageto);
	Imageto.release();
}

void FeatureMatcher::drawStitchedImage(vector<MatchesInfo> &_pairwise_matches, const std::vector<cv::Mat> &_images) {
	for (MatchesInfo M : _pairwise_matches) {
		if (M.src_img_idx < 0 || M.dst_img_idx < 0) continue;
		if (M.num_inliers <= 0) continue;
		Mat Img1 = _images[M.src_img_idx].clone();
		Mat Img2 = _images[M.dst_img_idx].clone();
		Mat merged;
		warpPerspective(Img1, merged, M.H, Size(Img2.cols, Img2.rows + Img1.rows));
		merged.rowRange(0, Img2.rows) = Img2 * 1;
		Mat original, Matched;
		String fileName = OUTPUTPATH + "STITCHED_" + to_string(M.src_img_idx) + "_" + to_string(M.dst_img_idx) + ".jpg";
		imwrite(fileName, merged);
	}
}