#ifndef FEATUREEXTRACTOR_H
#define FEATUREEXTRACTOR_H

#include <string>
#include <cstring>
#include <dlib/array2d.h>
#include <dlib/image_keypoint.h>
#include <dlib/image_io.h>
#include <dlib/image_keypoint/draw_surf_points.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/stitching/detail/matchers.hpp>
#include <boost/filesystem.hpp>
#include "ProjectProperty/include/ProjectProperty.hpp"
#include "ProjectProperty/include/Config.hpp"

class FeatureExtractor{
      public :
            FeatureExtractor (const std::string & _fileName);
            void extractKeyPoint(std::vector<cv::KeyPoint> & _keyPointList, cv::Mat &_Descriptor);	      
            void extractDescriptor(std::vector<cv::KeyPoint> & _keyPointList, cv::Mat & _descriptor);	
            int  DisplayKeyPoints(const std::vector<cv::KeyPoint> & _keyPointList);	 
            int  getImgHeight();
            int  getImgWidth();     
      private :
            Extractor_Type extractor_type;
            std::string fileName;
            int height,width;
            Image_Type getImageFormat(const std::string & _fileName );
};
#endif