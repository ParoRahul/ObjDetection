#ifndef CONFIG_H
#define CONFIG_H

//#define BOOST_LIB_DIAGNOSTIC
#include <iostream>
#include <map>
#include <assert.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include "ProjectProperty/include/ProjectProperty.hpp"
#include "ProjectProperty/include/ConfigEntry.hpp"

class Config
{
public:
	Config();

	~Config();

	static Config* getInstance();

	static void setInstance(Config* newInst);

	static void getConfigFile(const std::string _directoryName);

	static void LoadProfiles();

	static void loadConfig();

	bool loadFromFile();

	static int getInt(const std::string& id);

	static bool getBool(const std::string& id);

	static std::string getString(const std::string& id);

	static float getFloat(const std::string& id);

	static bool getFloatMinMax(const std::string& id, std::pair<float, float>& minmax);

	static bool getIntMinMax(const std::string& id, std::pair<int, int>& minmax);

	void clear();

	static Config* m_inst;

	static std::map<std::string, ConfigEntry > m_values;

	static std::vector<std::string> profilesToLoad;

	static std::string DirectoryName;

private:
	static boost::mutex Mutex;

	template<class T> T get(const std::string& id) const;

	template<class T> bool getMinMax(const std::string& id, std::pair<T, T>& minmax) const;

	std::string m_fileName;

};

template<> inline float Config::get(const std::string& id) const
{
	boost::mutex::scoped_lock lock(Mutex);
	std::map<std::string, ConfigEntry >::const_iterator it = m_values.find(id);
	if (it == m_values.end())
	{
		LOGEROR("Config: ERROR Config value '" << id << "' not found! Exiting!");
	}
	assert(it != m_values.end());

	ConfigEntry::eType t = it->second.getType();

	if (t != ConfigEntry::TYPE_FLOAT)
	{
		std::cerr << "Config: ERROR Config value '" << id << "' has wrong type! Exiting!" << std::endl;
	}
	assert(t == ConfigEntry::TYPE_FLOAT);
	float value = it->second.getFloat();
	return value;
}

template<>inline int Config::get(const std::string& id) const
{
	boost::mutex::scoped_lock lock(Mutex);
	std::map<std::string, ConfigEntry>::const_iterator it = m_values.find(id);
	if (it == m_values.end())
	{
		std::cerr << "Config: ERROR Config value '" << id << "' not found! Exiting!" << std::endl;
	}
	assert(it != m_values.end());

	ConfigEntry::eType t = it->second.getType();
	if (t != ConfigEntry::TYPE_INT)
	{
		std::cerr << "Config: ERROR Config value '" << id << "' has wrong type! Exiting!" << std::endl;
	}
	int value = it->second.getInt();
	assert(t == ConfigEntry::TYPE_INT);
	return value;
}

template<>inline const char* Config::get(const std::string& id) const
{
	boost::mutex::scoped_lock lock(Mutex);
	std::map<std::string, ConfigEntry>::const_iterator it = m_values.find(id);
	if (it == m_values.end())
	{
		std::cerr << "Config: ERROR Config value '" << id << "' not found! Exiting!" << std::endl;
	}
	assert(it != m_values.end());

    ConfigEntry::eType t = it->second.getType();
	if (t != ConfigEntry::TYPE_STRING)
	{
		std::cerr << "Config: ERROR Config value '" << id << "' has wrong type! Exiting!" << std::endl;
	}
	assert(t == ConfigEntry::TYPE_STRING);
	const char* value = it->second.getString();
	return value;
}

template<>inline std::string Config::get(const std::string& id) const
{
	return get<const char*>(id);
}

template<class T> bool Config::getMinMax(const std::string& id, std::pair<T, T>& minmax) const
{
	std::map<std::string, ConfigEntry>::const_iterator it = m_values.find(id);
	if (it == m_values.end())
	{
		std::cerr << "Config: ERROR Config value '" << id << "' not found! Exiting!" << std::endl;
	}
	assert(it != m_values.end());

	ConfigEntry::eType t = it->second.getType();
	if (t == ConfigEntry::TYPE_INT)
	{
		int mi = it->second.getIntMin();
		int ma = it->second.getIntMax();
		if (mi == ma) return false;
		minmax = std::make_pair(mi, ma);
		return true;
	}
	else if (t == ConfigEntry::TYPE_FLOAT)
	{
		float mi = it->second.getFloatMin();
		float ma = it->second.getFloatMax();
		if (mi == ma) return false;
		minmax = std::make_pair(mi, ma);
		return true;
	}
	return false;
}

#endif