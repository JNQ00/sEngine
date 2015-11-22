#ifndef __COMMON_TOOLS_H__
#define __COMMON_TOOLS_H__

#include <Windows.h>
#include <float.h>
#include <cmath>
#include <string>

namespace Common{
	
//---------------------------------------------------------------------------------------
// Convenience macro for releasing COM objects.
//---------------------------------------------------------------------------------------

#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }

//---------------------------------------------------------------------------------------
// Convenience macro for deleting objects.
//---------------------------------------------------------------------------------------

#define SafeDelete(x) { if (x) {delete x; x = 0;} }


//-------------------------------------------------------------
enum ERelativeSpace
{
	ERelativeSpace_PARENT,
	ERelativeSpace_LOCAL,
	ERelativeSpace_WORLD,
	ERelativeSpace_COUNT
};

class MathHelper
{
public:
	// Returns random float in [0, 1).
	static float RandF()
	{
		return (float)(rand()) / (float)RAND_MAX;
	}

	// Returns random float in [a, b).
	static float RandF(float a, float b)
	{
		return a + RandF()*(b-a);
	}

	// Returns random unsigned in [a, b).
	static unsigned RandI(unsigned a, unsigned b)
	{
		return (unsigned)(a + RandF()*(b-a));
	}

	template<typename T>
	static T Min(const T& a, const T& b)
	{
		return a < b ? a : b;
	}

	template<typename T>
	static T Max(const T& a, const T& b)
	{
		return a > b ? a : b;
	}

	template<typename T>
	static T Lerp(const T& a, const T& b, float t)
	{
		return a + (b-a)*t;
	}

	template<typename T>
	static T Clamp(const T& x, const T& low, const T& high)
	{
		return x < low ? low : (x > high ? high : x); 
	}

	// Returns the polar angle of the point (x,y) in [0, 2*PI).
	static float AngleFromXY(float x, float y);

	static const float Infinity;
	static const float Pi;
	static const float Pi_2;
	static const float Pi_4;
	
};

class GUIDGenerator
{
public:
	static std::string generateGUID(const char* _base = NULL);
};

class FileHelpers
{
public:
	static std::string getFileExtention(const char* _name);
	static std::string getFileExtention(const std::string& _name);

	static std::string getFilePath(const char* _name);
	static std::string getFilePath(const std::string& _name);
};

struct seTime
{
	float gameTime;
	float globalTime;
	float deltaGameTime;
	float deltaGlobalTime;
};

class TimeHelpers
{
public:
	static long int getCurrentTime();
	static float getCurrentTimeMillis();
};

}


#endif
