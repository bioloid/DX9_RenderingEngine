#ifndef __FPS_H__
#define __FPS_H__
#include "TIME.h"
#define INST true
#define AVG false
#define RESETCOUNT 256
class FPS
{
private:
	TIME time;
	bool mode;
	unsigned int frameCount;
public:
	FPS();
	~FPS();
	void Initialize(bool);
	void Reset();
	void Count();
	float Get();
};

#endif