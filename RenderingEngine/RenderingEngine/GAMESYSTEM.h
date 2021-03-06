#ifndef __GAMESYSTEM_H__
#define __GAMESYSTEM_H__
#define MSGPRINTMAXSIZE 128

#ifdef _DEBUG
#define CONSOLEUSE true
#else
#define CONSOLEUSE false
#endif


#ifdef DEBUGCONSOLE
#define CONDEBUG true
#endif

#define DEFAULTVER "1.8.0"
#define DEFAULTBUILDNUMBER 1

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
#include "MODEL.h"
#include "LIGHT.h"
#include "TEXTURERENDER.h"

//	Other Includes
//
#include "RUNTIME_ERROR.h"

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
	string				pName;
	MSG					msg;

	//	DirectX
	//
	IDirect3DDevice9*	device = NULL;
	ID3DXFont*			font = NULL;

	//	Shader
	//
	LPD3DXEFFECT		shadowZBuildShader = NULL;
	LPD3DXEFFECT		shadowHorizontalBlurShader = NULL;
	LPD3DXEFFECT		shadowVerticalBlurShader = NULL;
	LPD3DXEFFECT		shadowBlackWhiteBuildShader = NULL;
	LPD3DXEFFECT		lastSceneShader = NULL;
	LPD3DXEFFECT		SamplingShader = NULL;

	LPDIRECT3DTEXTURE9		shadowZBuildRT = NULL;
	LPDIRECT3DSURFACE9		shadowZBuildStencil = NULL;
	LPDIRECT3DTEXTURE9		shadowBlackWhiteBuildRT = NULL;
	LPDIRECT3DTEXTURE9		shadowVerticalBlurRT = NULL;
	LPDIRECT3DTEXTURE9		shadowHorizontalBlurRT = NULL;


	LPDIRECT3DSURFACE9 backUpMainRT = NULL;
	LPDIRECT3DSURFACE9 bakcUpMainStencil = NULL;
	LPDIRECT3DSURFACE9 tmpSurface = NULL;
	UINT shaderNumPass;


	DWORD				savedFVF = 0UL;	// MODEL::VertexXYZ::FVF;
	IDirect3DTexture9**	default_texture = NULL;
	map<string, IDirect3DTexture9*> texture;

	//	Font
	//
	char				str[MSGPRINTMAXSIZE];
	RECT				FontBox;

	//	ETC
	//
	RECT				winSize = { 0, 0, 1280, 720 };
	RECT				conSize = { 0, 0, 600, 720 };
	char				pPath[MAX_PATH];	// MAX_PATH from Windows.h Header
	int					pEndWaitTime = 3000;	// Engine END time in ms
	bool				bConUsage = CONSOLEUSE;

public:
	bool				bRunGame = true;

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
	TEXTURERENDER		downSampler, upSampler;

	MODEL				floor, screen;
	MODEL				box0, box1, box2, box3;
	LIGHT				testLight;



	//	Functions
	//
public:
	void Initialize(HINSTANCE, HINSTANCE, LPSTR, int);
	void D3DInitialize(bool, D3DDEVTYPE);
private:
	void D3DEffectInitialize();
	void Render();
	void DrawMSG();
	void MSGPrint(int, int);

	void UpSamping();
	void DownSamping();
	void ShadowBuild();
	void HorizontalBlur();
	void VerticalBlur();
	void DrawScene();
	void LightZBuild();
	void StartScene();
	void EndScene();

	void ShaderLoad(string _name, LPD3DXEFFECT& shader);
	void Test();

public:
	void Release();
	void D3DRelease();
	void Run();
	void EndGame();
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
	

	GAMESYSTEM();
	~GAMESYSTEM();

	friend class CPU;
	friend class MODEL;
	friend class KEYBOARD;
	friend class CONSOLE;
	friend class CAMERA;
	friend class MOUSE;
	friend class RUNTIME_ERROR;
	friend class LIGHT;
	friend class TEXTURERENDER;
};

//	Other Variables
//
extern GAMESYSTEM gSystem;


template<class T> void Release_(T t)
{
	if (t != NULL)
	{
		t->Release();
		t = NULL;
	}
}
#endif // !__GAMESYSTEM_H__