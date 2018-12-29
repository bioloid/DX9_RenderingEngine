//	Debug Includes
//
#include "CRT_DEBUG_.h"

//	Game Includes
//
#include "GAMESYSTEM.h"

//	WINAPI includes
//
#include <Windows.h>

//	CPP includes
//
#include <iostream>

//	Global Variables
//
GAMESYSTEM gSystem;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR cmdLine, int showCmd)
{
	try {
		gSystem.Initialize(hInstance, prevInstance, cmdLine, showCmd, 8);
	}
	catch (std::exception &e) {
		return 1;
	}
	gSystem.Run();


	gSystem.Release();


	return 0;
}