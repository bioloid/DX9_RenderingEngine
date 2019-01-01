#ifndef __GAMESYSTEM_H__
#define __GAMESYSTEM_H__
#define MSGPRINTMAXSIZE 128

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
#include "TIME.h"
#include "FPS.h"
#include "CPU.h"
#include "RAM.h"
#include "CAMERA.h"


class GAMESYSTEM
{
private:
//	Windows API
//	
	HWND				hwnd;
	HINSTANCE			hInstance;
	HINSTANCE			prevInstance;
	LPSTR				cmdLine;
	WNDCLASS			wc;
	int					showCmd;
	const TCHAR*		pName			= "Rendering Engine";
	MSG					msg;

	
//	DirectX
//
	IDirect3DDevice9*	device			= NULL;
	ID3DXFont*			font			= NULL;
	LPD3DXEFFECT		shader			= NULL;
	D3DXMATRIXA16		worldmatrix;
	D3DXMATRIXA16		viewmatrix;
	D3DXMATRIXA16		projmatrix;


//	Font
//
	char				str[MSGPRINTMAXSIZE];
	RECT				FontBox;


//	ETC
//
	unsigned int		check;
	RECT				windowSize		= { 0, 0, 1280, 720 };
	RECT				consoleSize		= { 0, 0, 600, 720 };
	char				pPath[MAX_PATH];	// MAX_PATH from Windows.h Header
	int					pEndTime		= 3000;	// Engine END time in ms
	bool				consoleDebug	= CONSOLEDEBUG;
	bool				debug			= false;


public:
	bool				runGame = true;

private:
//	ETC Class
//
	CONSOLE				console;
	MEMORY				memory;
	KEYBOARD			keyboard;
	MOUSE				mouse;
	TIME				time;
	FPS					avgfps;
	FPS					instfps;
	CPU					cpu;
	RAM					ram;
	CAMERA				camera;




//	Functions
//

private:
	void IN_Initialize(HINSTANCE, HINSTANCE, LPSTR, int);
	
	void IN_D3DInitialize(bool, D3DDEVTYPE);
	void IN_D3DEffectInitialize();
	void IN_D3DRelease();
	void Render();

	void DrawMSG();
	void MSGPrint(int, int);

	void Test();


public:
	void Release();
	GAMESYSTEM();
	~GAMESYSTEM();
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
	void Initialize(HINSTANCE, HINSTANCE, LPSTR, int, int);
	void D3DInitialize(bool, D3DDEVTYPE, int);
	void Run();
	void EndGame();



	friend class KEYBOARD;
	friend class CONSOLE;
	friend class CAMERA;
	friend class MOUSE;
};


//	Other Variables
//
extern GAMESYSTEM gSystem;



#endif // !__GAMESYSTEM_H__