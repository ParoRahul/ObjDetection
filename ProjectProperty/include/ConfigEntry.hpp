#ifndef CONFIGENTRY_H
#define CONFIGENTRY_H
#include <string>
#include <cstring>
#include <assert.h>

class ConfigEntry {
public:
	ConfigEntry();
	
	~ConfigEntry();

	//ConfigEntry(const ConfigEntry& e2);

	ConfigEntry& operator= (const ConfigEntry& e2);

	//bool operator==(const ConfigEntry& e2);

	template<class T> void set(T t);
	
	void setInt(int i);

	void setIntMin(int m);
	
	void setIntMax(int m);
	
	void setFloat(float f);
	
	void setFloatMin(float m);
	
	void setFloatMax(float m);
	
	void setString(const char* p);
	
	int getInt() const;
	
	float getFloat() const;
	
	const char* getString() const;
	
	int getIntMax() const;
	
	int getIntMin() const;
	
	float getFloatMax() const;
	
	float getFloatMin() const;
	
	enum eType
	{
		TYPE_INT = 1,
		TYPE_FLOAT,
		TYPE_STRING,
	};

	eType getType() const
	{
		return type;
	}

private:

	eType deduceType(int) const
	{
		return TYPE_INT;
	}
	eType deduceType(const char*) const
	{
		return TYPE_STRING;
	}
	eType deduceType(float) const
	{
		return TYPE_FLOAT;
	}
	eType type;
	union
	{
		char* data;
		int data_int;
		float data_float;
	};
	union
	{
		void* minval;
		int minval_int;
		float minval_float;
	};
	union
	{
		void* maxval;
		int maxval_int;
		float maxval_float;
	};
};
#endif
