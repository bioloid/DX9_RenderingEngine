#ifndef __GAMESYSTEM_H__
#define __GAMESYSTEM_H__
#include <d3d9.h>
#include <d3dx9.h>

#include <Windows.h>
#include <iostream>
#include <stdexcept>

//#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")
class GAMESYSTEM
{
private:
//	Windows API Variables
//
	HWND hwnd;
	HINSTANCE hInstance;
	HINSTANCE prevInstance;
	LPSTR cmdLine;
	WNDCLASS wc;
	int showCmd;
	const TCHAR* pName = "Rendering Engine v1.0.0";
	MSG msg;
	HWND consoleHND;

//	ETC Variables
//
	unsigned int check;
	RECT windowSize = { 0, 0, 1280, 720 };
	RECT consoleSize = { 0, 0, 600, 720 };
	char pPath[MAX_PATH];	// MAX_PATH from Windows.h Header
	int pEndTime = 3000;	// Engine END time in ms
public:
	bool runGame = true;
		
//	Functions
//
	void IN_Initialize(HINSTANCE, HINSTANCE, LPSTR, int);

public:

	GAMESYSTEM();
	~GAMESYSTEM();
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
	void Initialize(HINSTANCE, HINSTANCE, LPSTR, int, int);
	void Run();
	void Release();
};

#endif // !__GAMESYSTEM_H__
