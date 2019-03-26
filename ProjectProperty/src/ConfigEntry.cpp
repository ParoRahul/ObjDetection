#include "ProjectProperty/include/ConfigEntry.hpp"

ConfigEntry::ConfigEntry() { data = 0; minval = 0; maxval = 0; type = TYPE_INT; }

ConfigEntry::~ConfigEntry() {
	if (type == TYPE_STRING) {
		delete[] data;
	}
}
/*
ConfigEntry::ConfigEntry(const ConfigEntry& e2)
{
	type = e2.type;
	minval = e2.minval;
	maxval = e2.maxval;
	if (e2.type != TYPE_STRING)
	{
		data = e2.data;
	}
	else
	{
		data = new char[strlen((const char*)e2.data) + 1];
		std::strcpy((char*)data, (const char*)e2.data);
	}
}
*/
ConfigEntry& ConfigEntry::operator= (const ConfigEntry& e2)
{
	type = e2.type;
	minval = e2.minval;
	maxval = e2.maxval;
	if (e2.type != TYPE_STRING)
	{
		data = e2.data;
	}
	else
	{
		data = new char[strlen((const char*)e2.data) + 1];
		std::strcpy((char*)data, (char*)e2.data);
	}
	return (*this);
}
/*
bool ConfigEntry::operator==(const ConfigEntry& e2)
{
	//std::cout << "types: " << e2.type << " " << type << std::endl;
	if (e2.type != type) return false;
	if (e2.type != TYPE_STRING)
	{
		bool ret = (e2.data == data) && (e2.minval == minval) && (e2.maxval == maxval);;
		//std::cout << e2.data << " " << data << " returning " << ret << std::endl;
		return ret;
	}
	else
	{
		if (e2.data&&data)
		{
			return !strcmp((const char*)e2.data, (const char*)data);
		}
		else return false;
	}
}
*/

template<> inline void ConfigEntry::set(float t)
{
	setFloat(t);
}

template<>inline void ConfigEntry::set(int t)
{
	setInt(t);
}

template<>inline void ConfigEntry::set(const char* t)
{
	setString(t);
}

void ConfigEntry::setInt(int i)
{
	type = TYPE_INT;
	data_int = i;
}

void ConfigEntry::setIntMin(int m)
{
	assert(type == TYPE_INT);
	minval_int = m;
}
void ConfigEntry::setIntMax(int m)
{
	assert(type == TYPE_INT);
	maxval_int = m;
}
void ConfigEntry::setFloat(float f)
{
	type = TYPE_FLOAT;
	data_float = f;
}
void ConfigEntry::setFloatMin(float m)
{
	assert(type == TYPE_FLOAT);
	minval_float = m;
}
void ConfigEntry::setFloatMax(float m)
{
	assert(type == TYPE_FLOAT);
	maxval_float = m;
}
void ConfigEntry::setString(const char* p)
{
	type = TYPE_STRING;
	delete[](char*) data;
	data = new char[strlen(p) + 1];
	std::strcpy((char*)data, p);
}
int ConfigEntry::getInt() const
{
	assert(type == TYPE_INT);
	return data_int;
}
float ConfigEntry::getFloat() const
{
	assert(type == TYPE_FLOAT);
	return data_float;
}
const char* ConfigEntry::getString() const
{
	assert(type == TYPE_STRING);
	return (const char*)data;
}
int ConfigEntry::getIntMax() const
{
	assert(type == TYPE_INT);
	return maxval_int;
}
int ConfigEntry::getIntMin() const
{
	assert(type == TYPE_INT);
	return minval_int;
}
float ConfigEntry::getFloatMax() const
{
	assert(type == TYPE_FLOAT);
	return maxval_float;
}
float ConfigEntry::getFloatMin() const
{
	assert(type == TYPE_FLOAT);
	return minval_float;
}
