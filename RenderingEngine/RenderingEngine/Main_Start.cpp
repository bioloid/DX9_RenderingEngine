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
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//	_CrtSetBreakAlloc(469);
#endif

	gSystem.Initialize(hInstance, prevInstance, cmdLine, showCmd);
	gSystem.D3DInitialize(true, D3DDEVTYPE_HAL);


	cout << ">> ";
	gSystem.Run();
	gSystem.D3DRelease();
	gSystem.Release();

	return 0;
}