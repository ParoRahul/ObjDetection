/*******************************************************************************
 *  DBModule.hpp
 *
 *
 *******************************************************************************/

#ifndef DBMODULE_H
#define DBMODULE_H

#include <sys/stat.h>
#include "ProjectProperty/include/Config.hpp"
#include "DBModule/include/ObjectProperties.hpp"
#include <sstream>
#include <fstream>

class DBModule
{
  public:

    /** @brief The constructor. */
	DBModule(std::string action);

    /** @brief The destructor. */
    virtual ~DBModule();

    void loadObjectList(std::vector<ObjectProperties> &objectPropertyList);

	void createDBObjectList();		


  protected:

    //void loadImageFiles(std::string &_action);	

	ObjectProperties loadDBObjects( std::string filename ); // TODO move back to private

	void createDBojects(std::string _filename);

	inline bool validateImageFormat(const std::string &fileName);

	inline std::string createArchivefiename(const std::string &fileName);

	std::map<std::string,std::string> ImageListDBMap;
	
	std::string DBImagePath,DBDataPath;
    
	std::string Action;
};

#endif