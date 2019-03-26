#include "FeatureExtraction/include/FeatureExtractor.hpp"

FeatureExtractor::FeatureExtractor( const std::string & _fileName )
                :fileName(_fileName),height(0),width(0){
	switch (Config::getInt("ObjectRecognition.ExtractorType")){
        case 0:{
                extractor_type=SURF_DLIB;
                break ;}
        case 1:{
                extractor_type=SIFT_VLFET;
                break;}
        case 2:{      
                extractor_type=BRISK;
                break;}
        default: 
                  LOGEROR(" INVALID FeatureExtractor  ");
        }                  
}

void FeatureExtractor::extractKeyPoint(std::vector<cv::KeyPoint> & _keyPointList, cv::Mat &_Descriptor){
        if(!boost::filesystem::exists(fileName))
           {
               LOGEROR("File not found: "+fileName);
               return;
           }
        switch( extractor_type)
        {
        case SURF_DLIB:
	{
             dlib::array2d<dlib::rgb_pixel> img;
             int classid=1;   
             //Image_Type Imgtype = getImageFormat(fileName);
	     double detection_threshold = Config::getFloat("Surf.DetectionThreshold");
	     int max_points = Config::getInt("Surf.NumPoints");
             dlib::load_image(img,fileName);             
             height= img.nr();width= img.nc();
             if ( height <= 0 || width <=0  ){
                  LOGEROR(" Invalid Image , Size "+std::to_string(height*width));
                  return ;
             }   
             std::vector<dlib::surf_point> sp = dlib::get_surf_points(img, max_points,detection_threshold);
             if (false){
                dlib::image_window my_window(img);
                dlib::draw_surf_points(my_window, sp);
                my_window.wait_until_closed();
             }             
             for ( unsigned i=0; i<sp.size(); i++){ 
                   cv::Point2f pt(sp[i].p.center(0),sp[i].p.center(1));                   
                   cv::KeyPoint kp(pt,sp[i].p.laplacian,sp[i].angle,sp[i].p.score,sp[i].p.scale,classid);  
                   cv::Mat Desciptor_row(1,64,CV_64FC1);
                   std::vector<double> Desciptor_vec(sp[i].des.begin(), sp[i].des.end());
                   std::memcpy(Desciptor_row.data, &Desciptor_vec, 64);
                   _Descriptor.push_back(Desciptor_row);
                   _keyPointList.push_back(kp);
                }
             break;
	}
        case SIFT_VLFET:
	{
             LOGINFO(" CODING FOR SIFT  NOT DONE ");
             break;
	}
        case INVALID:
	{
             LOGEROR(" INVALID FeatureExtractor  ");
             break;
	}
        default: 
                  LOGEROR(" INVALID FeatureExtractor  ");
        }
}


int FeatureExtractor::DisplayKeyPoints(const std::vector<cv::KeyPoint> & _keyPointList){
        try{
                //boost::filesystem::path imageFile(fileName);
                std::cout<<" Total number Of Key Point "<< _keyPointList.size() << std::endl;
                cv::Mat Temp,img;
		Temp= cv::imread(fileName,cv::IMREAD_UNCHANGED);		
		if (Temp.empty()){
		    //throw boost::filesystem::filesystem_error(" Invalid Image File "+fileName);	
                    LOGINFO(" Invalid Image File " + fileName);
                    return std::EXIT_FAILURE;
                }
		cv::drawKeypoints(Temp,_keyPointList, img,cv::Scalar::all(-1),cv::DrawMatchesFlags::DEFAULT );
                //std::string OutfileName=std::to_string(imageFile.stem())+"KeyPoints"+std::to_string(imageFile.extension());
                std::string OutfileName="outfile.jpg";
                std::string File2write = Config::getString("Path.DegubDir")+"/"+OutfileName;
                cv::imwrite(File2write,img);
                return EXIT_SUCCESS;
        }
        catch(const boost::filesystem::filesystem_error& e){
                LOGEROR( e.code().message() );
                return EXIT_FAILURE;
        }
        catch(const std::exception& e) {
                LOGEROR(" FeatureMatcher ");
                return EXIT_FAILURE;
        }
}

int  FeatureExtractor::getImgWidth(){
        return width;
}

int  FeatureExtractor::getImgHeight(){
        return height;
}

Image_Type FeatureExtractor::getImageFormat(const std::string & _fileName ){
        std::string extension = boost::filesystem::extension(_fileName);
        Image_Type img_type;
        if (extension == "JPG" || extension == "jpg" || extension == "jpeg" ||  extension == "jpe" )
            img_type = Image_Type::JPG;
        else if ( extension == "png" || extension == "PNG" )
            img_type = Image_Type::PNG;
        else
            img_type = Image_Type::INVALID;
        return img_type;
}