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
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	try {
		gSystem.Initialize(hInstance, prevInstance, cmdLine, showCmd, 8);
	}
	catch (std::exception &e) {
		return 1;
	}
	gSystem.Run();


	gSystem.EndGame();


	return 0;
}