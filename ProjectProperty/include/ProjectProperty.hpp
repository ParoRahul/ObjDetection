#ifndef PROJECTPROPERTY_H
#define PROJECTPROPERTY_H

#include <iostream>
#include <vector>
#include <string>

#define LOGINFO(info_msg) std::cout << info_msg << std::endl
#define LOGEROR(eror_msg) std::cerr << eror_msg << std::endl

const std::vector<std::string> Img_formats = {".jpeg", ".jpg", ".jpe", ".JPG",".jp2",".png", ".PNG"};

const std::string BinaryArchiveFormat = "dat";

const std::string TextArchiveFormat = "objprop";

const std::string Config_directory = "D:/CPLUSPLUS/VSC/Config";

enum Extractor_Type {
	SURF_DLIB ,
	SIFT_VLFET,
	BRISK
}; 

enum Image_Type {
	JPG,
    PNG,	
	INVALID
};
#endif