/*******************************************************************************
 *  ObjectProperties.hpp
 *
 *  
 *******************************************************************************/

#ifndef OBJECTPROPERTIES_H
#define OBJECTPROPERTIES_H

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
//#include <boost/archive/text_iarchive.hpp>
//#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/split_free.hpp>
#include <boost/filesystem.hpp>
//#include "../SURF/KeyPointExtractor/KeyPoint.h"
//#include "../SURF/KeyPointExtractor/DefaultExtractor.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/core/utility.hpp>
#include <fstream>
#include "ProjectProperty/include/ProjectProperty.hpp"
#include "FeatureExtraction/include/FeatureExtractor.hpp"

BOOST_SERIALIZATION_SPLIT_FREE(::cv::Mat)

class ObjectProperties {

  public:
	
    friend class boost::serialization::access;

    /** @brief Default constructor. */
    ObjectProperties( std::string name );

	ObjectProperties( ) = default;

    /** @brief Copy constructor */
    ObjectProperties( const ObjectProperties& other );

    /** @brief The destructor */
    ~ObjectProperties();

    /** @brief Assignment operator */
    ObjectProperties& operator= (const ObjectProperties& right);

	///** @brief Fetch object Property */
	//void featchObjectProperties();
    
    // SERIALIZATION

    template<class Archive>void serialize(Archive & ar, const unsigned int version)
    {
		ar & m_Name;
		ar & m_Identifier;
		ar & height;
		ar & width;
		ar & feature_count;
		for ( unsigned i=0; i<feature_count; i++)
        {
			ar & featureList[i].pt.x;
			ar & featureList[i].pt.y;
			ar & featureList[i].size;
			ar & featureList[i].angle;
			ar & featureList[i].response;
			ar & featureList[i].octave;
        }
		size_t elem_size = Descriptor.elemSize();
		size_t elem_type = Descriptor.type();
		ar & Descriptor.cols;
		ar & Descriptor.rows;
		ar & elem_size;
		ar & elem_type;
		const size_t data_size = Descriptor.cols * Descriptor.rows * elem_size;
		ar & boost::serialization::make_array(Descriptor.ptr(), data_size);
    }


    // template<class Archive>void save(Archive & ar, const unsigned int version) const
    // {
	// 	ar & m_Name;
	// 	ar & m_Identifier;
	// 	ar & height;
	// 	ar & width;
	// 	ar & feature_count;
	// 	for ( unsigned i=0; i<feature_count; i++)
    //     {
	// 		ar & featureList[i].pt.x;
	// 		ar & featureList[i].pt.y;
	// 		ar & featureList[i].size;
	// 		ar & featureList[i].angle;
	// 		ar & featureList[i].response;
	// 		ar & featureList[i].octave;
    //     }
	// 	size_t elem_size = Descriptor.elemSize();
	// 	size_t elem_type = Descriptor.type();
	// 	ar & Descriptor.cols;
	// 	ar & Descriptor.rows;
	// 	ar & elem_size;
	// 	ar & elem_type;
	// 	const size_t data_size = Descriptor.cols * Descriptor.rows * elem_size;
	// 	ar & boost::serialization::make_array(Descriptor.ptr(), data_size);
    // }

    // template<class Archive> void load(Archive & ar, const unsigned int version_b)
    // {
    //     ar & m_Name;
    //     ar & m_Identifier;
	// 	ar & height;
	// 	ar & width;
	// 	ar & feature_count;
	// 	featureList.reserve(feature_count);
    //     for ( unsigned i=0; i < feature_count; i++)
    //     {
	// 		cv::KeyPoint Kp;
    //         ar & Kp.pt.x;
	// 		ar & Kp.pt.y;
	// 		ar & Kp.size;
	// 		ar & Kp.angle;
	// 		ar & Kp.response;
	// 		ar & Kp.octave;
	// 		featureList.push_back(Kp);
    //     }
	// 	int cols, rows;
	// 	size_t elem_size, elem_type;
	// 	ar & cols;
	// 	ar & rows;
	// 	ar & elem_size;
	// 	ar & elem_type;
	// 	Descriptor.create(rows, cols, elem_type);
	// 	size_t data_size = Descriptor.cols * Descriptor.rows * elem_size;
	// 	ar & boost::serialization::make_array(Descriptor.ptr(), data_size);
    // }

    // BOOST_SERIALIZATION_SPLIT_MEMBER()

    /** @brief Print object information */

    void printOn( std::ostream& strm );
 
    std::string getName();

	int getHeight();

	int getwidth();

    std::vector<cv::KeyPoint> getKPlist();

	cv::Mat getDescriptor();

  private:

    std::string m_Name;

	std::string m_Identifier;

	int height;

	int width;

	std::string feature_type;

	int feature_count;

	std::vector<cv::KeyPoint> featureList;

	cv::Mat Descriptor;

};

#endif
