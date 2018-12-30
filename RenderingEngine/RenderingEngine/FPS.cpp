#include "FPS.h"

FPS::FPS()
{
}

FPS::~FPS()
{
}

void FPS::Initialize(bool _mode)
{
	mode = _mode;
	time.Initialize();
	frameCount = 0;
}
void FPS::Reset()
{
	time.ResetTime();
	frameCount = 0;
}
void FPS::Count()
{
	if (mode == INST)
	{
		if (frameCount > RESETCOUNT)
		{
			frameCount = 0;
			time.ResetTime();
		}
		else
		{
			frameCount++;
		}
	}
	else
	{
		frameCount++;
	}
}
float FPS::Get()
{
	return frameCount / (time.getTime()+0.001f);
}