#include <Windows.h>
#include <ctime>

#include "CommonTools.h"
#include <sstream>
#include <assert.h>
#include "Log.hpp"


namespace Common{

const float MathHelper::Infinity = FLT_MAX;
const float MathHelper::Pi       = 3.1415926535f;
const float MathHelper::Pi_2       = 1.570796327f;
const float MathHelper::Pi_4       = 0.7853981634f;

float MathHelper::AngleFromXY(float x, float y)
{
	float theta = 0.0f;

	// Quadrant I or IV
	if(x >= 0.0f) 
	{
		// If x = 0, then atanf(y/x) = +pi/2 if y > 0
		//                atanf(y/x) = -pi/2 if y < 0
		theta = atanf(y / x); // in [-pi/2, +pi/2]

		if(theta < 0.0f)
			theta += 2.0f*Pi; // in [0, 2*pi).
	}

	// Quadrant II or III
	else      
		theta = atanf(y/x) + Pi; // in [0, 2*pi).

	return theta;
}

std::string GUIDGenerator::generateGUID(const char* _base /* = NULL */)
{
	unsigned salt = Common::MathHelper::RandI(0, 100);
	unsigned tstamp = (unsigned)time(NULL);

	std::stringstream ss;

	if ( _base )
	{
		ss << _base;
	}
	ss << salt << tstamp;
	return ss.str();
}

std::string FileHelpers::getFileExtention(const char* _name)
{
	assert(_name);
	return FileHelpers::getFileExtention(std::string(_name));
}

std::string FileHelpers::getFileExtention(const std::string& _name)
{
	size_t dotIndex = _name.find_last_of(".");
	if ( dotIndex == std::string::npos)
	{
		Log::i("FileHelpers") << "Extention not found for filename " << _name.c_str();
		return std::string();
	}
	return _name.substr(dotIndex+1, _name.size());
}

std::string FileHelpers::getFilePath(const char* _name)
{
	assert(_name);
	return FileHelpers::getFilePath(std::string(_name));
}

std::string FileHelpers::getFilePath(const std::string& _name)
{
	size_t slashIndex = _name.find_last_of("/");
	if ( slashIndex == std::string::npos)
	{
		Log::i("FileHelpers") << "Path not found for filename " << _name.c_str();
		return std::string();
	}
	return _name.substr(0, slashIndex+1);
}

long int TimeHelpers::getCurrentTime()
{
	return (long int)time(NULL);
}

float TimeHelpers::getCurrentTimeMillis()
{
	long ticks = (long)clock();
	float ms = (float)ticks / CLOCKS_PER_SEC;
	return ms;
}

}