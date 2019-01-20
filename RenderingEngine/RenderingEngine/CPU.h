#ifndef __CPU_H__
#define __CPU_H__
#pragma comment(lib, "pdh.lib")
#include <pdh.h>
#include "windows.h"
#define REFRESHTIME 1000
class CPU
{
public:
	CPU();
	~CPU();
	void Initialize();
	void Release();
	void Update();
	int GetCpuUsage();
	float GetProcessUsage();

private:
	ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
	int numProcessors;
	HANDLE self;
	SYSTEM_INFO sysInfo;
	FILETIME ftime, fsys, fuser;
	ULARGE_INTEGER now, sys, user;
	float percent;

	bool m_canReadCpu;
	HQUERY m_queryHandle;
	HCOUNTER m_counterHandle;
	unsigned long m_lastSampleTime;
	long m_cpuUsage;
};
#endif