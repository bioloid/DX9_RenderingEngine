#ifndef __TIME_H__
#define __TIME_H__
#include <Windows.h>
class TIME
{
private:
	ULONGLONG startTime;
public:
	TIME();
	~TIME();
	void Initialize();
	void ResetTime();
	float getTime();
};
#endif