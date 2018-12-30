#include "TIME.h"
#include <Windows.h>

TIME::TIME()
{
}


TIME::~TIME()
{
}

void TIME::Initialize()
{
	startTime = GetTickCount64();
}
void TIME::ResetTime()
{
	startTime = GetTickCount64();
}
float TIME::getTime()
{
	return (GetTickCount64() - startTime) * 0.001f;
}