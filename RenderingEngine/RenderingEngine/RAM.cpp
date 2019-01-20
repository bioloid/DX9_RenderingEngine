#include "RAM.h"
#include <iostream>

RAM::RAM()
{
}

RAM::~RAM()
{
}
void RAM::Initialize()
{
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS *)&g_mc, sizeof(g_mc));
}
void RAM::Update()
{
	if ((m_lastSampleTime + REFRESHTIME) < GetTickCount())
	{
		m_lastSampleTime = GetTickCount();

		memInfo.dwLength = sizeof(MEMORYSTATUSEX);
		GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS *)&pmc, sizeof(pmc));
		GlobalMemoryStatusEx(&memInfo);
		totalVirtualMem = memInfo.ullTotalPageFile;
		virtualMemUsed = memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;
		virtualMemUsedByMe = pmc.PrivateUsage;
		totalPhysMem = memInfo.ullTotalPhys;
		physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
		physMemUsedByMe = pmc.WorkingSetSize;
	}
}
DWORDLONG RAM::GetTotalVirtualMEM() { return totalVirtualMem; }
DWORDLONG RAM::GetUsedVirtualMEM() { return virtualMemUsed; }
DWORDLONG RAM::GetProcUsedVirtualMEM() { return virtualMemUsedByMe; }
DWORDLONG RAM::GetTotalMEM() { return totalPhysMem; }
DWORDLONG RAM::GetUsedMEM() { return physMemUsed; }
DWORDLONG RAM::GetProcUsedMEM() { return physMemUsedByMe; }