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

#pragma warning(disable:4244)
#pragma warning(disable:4477)


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR cmdLine, int showCmd)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	try {
		gSystem.Initialize(hInstance, prevInstance, cmdLine, showCmd, 8);
	}
	catch (std::exception &e) {
		cout << e.what() << endl;
		return 1;
	}
	try {
		gSystem.D3DInitialize(true, D3DDEVTYPE_HAL, 8);
	}
	catch (std::exception &e) {
		cout << e.what() << endl;
		return 1;
	}
	cout << ">> ";
	gSystem.Run();


	gSystem.Release();


	return 0;
}