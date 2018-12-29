#ifndef __GAMESYSTEM_H__
#define __GAMESYSTEM_H__

#ifdef _DEBUG
#define CONSOLEDEBUG true
#endif
#ifndef _DEBUG
#define CONSOLEDEBUG false
#endif

//	DirectX Includes
//
#include <d3d9.h>
#include <d3dx9.h>

//	C++ Includes
//
#include <iostream>
#include <stdexcept>

//	Window API Includes
//
#include <Windows.h>

//	Class Includes
//
#include "CONSOLE.h"
#include "MEMORY.h"
#include "KEYBOARD.h"
#include "MOUSE.h"

class GAMESYSTEM
{
private:
//	Windows API
//	
	HWND			hwnd;
	HINSTANCE		hInstance;
	HINSTANCE		prevInstance;
	LPSTR			cmdLine;
	WNDCLASS		wc;
	int				showCmd;
	const TCHAR*	pName = "Rendering Engine v1.0.0";
	MSG				msg;
	HWND			consoleHND;

	
//	DirectX
//


//	ETC
//
	unsigned int	check;
	RECT			windowSize		= { 0, 0, 1280, 720 };
	RECT			consoleSize		= { 0, 0, 600, 720 };
	char			pPath[MAX_PATH];	// MAX_PATH from Windows.h Header
	int				pEndTime		= 3000;	// Engine END time in ms
	bool			consoleDebug	= CONSOLEDEBUG;
	bool			debug			= false;

public:
	bool			runGame = true;

private:
//	ETC Class
//
	CONSOLE			console;
	MEMORY			memory;
	KEYBOARD		keyboard;
	MOUSE			mouse;


//	Functions
//

private:
	void IN_Initialize(HINSTANCE, HINSTANCE, LPSTR, int);
	void Release();
	void Test();


public:
	GAMESYSTEM();
	~GAMESYSTEM();
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
	void Initialize(HINSTANCE, HINSTANCE, LPSTR, int, int);
	void Run();
	void EndGame();




	friend class CONSOLE;
};


//	Other Variables
//
extern GAMESYSTEM gSystem;

#endif // !__GAMESYSTEM_H__