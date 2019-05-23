//	Includes
//
#include "GAMESYSTEM.h"

#include <string>
#include <fstream>

#pragma warning(disable:4477)
#pragma warning(disable:4996)
#pragma warning(disable:4244)


//	Functions
//
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
bool CtrlHandler(DWORD fdwCtrlType);

//	Private Functions
//
void GAMESYSTEM::D3DRelease()
{
	console.SetFunction("IN_D3DRelease");
	box0.Release();
	box1.Release();
	box2.Release();
	box3.Release();
	screen.Release();
	floor.Release();
	Release_<IDirect3DDevice9*>(device);
	console << con::info << con::func << "IDirect3DDevice9* Released" << con::endl;
	Release_<ID3DXFont*>(font);
	console << con::info << con::func << "ID3DXFont* Released" << con::endl;
	Release_<LPD3DXEFFECT>(shadowZBuildShader);
	Release_<LPD3DXEFFECT>(shadowHorizontalBlurShader);
	Release_<LPD3DXEFFECT>(shadowVerticalBlurShader);
	Release_<LPD3DXEFFECT>(shadowBlackWhiteBuildShader);
	Release_<LPD3DXEFFECT>(lastSceneShader);
	Release_<LPD3DXEFFECT>(SamplingShader);

	console << con::info << con::func << "LPD3DXEFFECT Released" << con::endl;

	Release_<LPDIRECT3DTEXTURE9>(shadowZBuildRT);
	Release_<LPDIRECT3DSURFACE9>(shadowZBuildStencil);
	Release_<LPDIRECT3DTEXTURE9>(shadowBlackWhiteBuildRT);
	Release_<LPDIRECT3DTEXTURE9>(shadowVerticalBlurRT);
	Release_<LPDIRECT3DTEXTURE9>(shadowHorizontalBlurRT);
	upSampler.Release();
	downSampler.Release();



	console.RestoreFunction();
}

void GAMESYSTEM::Release()
{
	console.SetFunction("Release");
	ShowWindow(hwnd, SW_SHOWMINIMIZED);
	for (auto ptr = texture.begin(); ptr != texture.end(); ptr++)
	{
		if (ptr->second != NULL)
			ptr->second->Release();
	}
	memory.Release();
	cpu.Release();

	pEndWaitTime = pEndWaitTime / 1000;
	while (pEndWaitTime > 0)
	{
		console << con::info << con::func << "End Program in " << pEndWaitTime << "sec\n";
		Sleep(1000);
		pEndWaitTime = pEndWaitTime - 1;
	}
}

void GAMESYSTEM::Initialize
(HINSTANCE _hInstance, HINSTANCE _prevInstance, LPSTR _cmdLine, int _showCmd)
{
	//	Command argc, argv
	//	Filename -c -d
	//

	if (__argc >= 2)
	{
		if (__argv[1] == "-c")
			bConUsage = true;
	}

	//	Class Initialization
	//
	console.SetFunction("GAMESYSTEM::Initialize");

	console.Initialize();
	keyboard.Initialize();
	mouse.Initialize();
	time.Initialize();
	cpu.Initialize();
	ram.Initialize();

	//	read program info
	//
	int build_number;
	string version;
	char build_number_tmp[16];
	
	ifstream version_read;
	ofstream version_write;

	version_read.open("ver.txt");
	version_read >> version >> build_number;
	build_number++;
	version_read.close();
	version_write.open("ver.txt", ios::trunc);
	version_write << version << '\n' << build_number;
	version_write.close();
	

	pName = "Rendering Engine " + version + " build " + string(itoa(build_number, build_number_tmp, 10));
	console << con::info << con::func << "program info is ready" << con::endl;

	//	Window Class Initialization
	//
	hInstance = _hInstance;	prevInstance = _prevInstance;
	cmdLine = _cmdLine;	showCmd = _showCmd;

	GetCurrentDirectory(MAX_PATH, pPath);
	ZeroMemory(&msg, sizeof(MSG));

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = pName.c_str();
	if (!RegisterClass(&wc))
	{
		console << con::info << con::func << "RegisterClass() - failed" << con::endl;
		throw RUNTIME_ERROR(CRITICAL_WINDOW_ERROR);
	}

	hwnd = CreateWindow(pName.c_str(), pName.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, winSize.right, winSize.bottom, 0, 0, hInstance, 0);
	if (!hwnd) 
	{
		console << con::info << con::func << "CreateWindow() - failed" << con::endl;
		throw RUNTIME_ERROR(CRITICAL_WINDOW_ERROR);
	}

	MoveWindow(hwnd, 0, 0, winSize.right, winSize.bottom, TRUE);
	ShowWindow(hwnd, SW_SHOW); //	SW_SHOW, SW_SHOWMAXIMIZED
	console.RestoreFunction();
}

void GAMESYSTEM::D3DEffectInitialize()
{
	console.SetFunction("D3DEffectInitialize");

	ShaderLoad("shader\\shadow.fx", shadowZBuildShader);
	ShaderLoad("shader\\HorizontalBlur.fx", shadowHorizontalBlurShader);
	ShaderLoad("shader\\VerticalBlur.fx", shadowVerticalBlurShader);
	ShaderLoad("shader\\SoftShadow.fx", shadowBlackWhiteBuildShader);
	ShaderLoad("shader\\MainShader.fx", lastSceneShader);
	ShaderLoad("shader\\SamplingShader.fx", SamplingShader);


	console << con::info << con::func << "shader Create succeed" << con::endl;
	console.RestoreFunction();
}
void GAMESYSTEM::ShaderLoad(string _name, LPD3DXEFFECT& shader)
{
	LPD3DXBUFFER error = NULL;
	DWORD shaderflags = 0;

#if _DEBUG
	shaderflags |= D3DXSHADER_DEBUG;
#endif

	D3DXCreateEffectFromFile(device, _name.c_str(), NULL, NULL, shaderflags, NULL, &shader, &error);
	if (!shader && error)
	{
		char *ack = (char*)error->GetBufferPointer();
		cout << ack << endl;
		getchar();
		console << con::error << con::func << "D3DXCreateEffectFromFile() error - " << _name << con::endl;
		throw RUNTIME_ERROR(CRITICAL_DIRECTX_ERROR);
	}
}


//	Public Functions
//
GAMESYSTEM::GAMESYSTEM()
{
}

GAMESYSTEM::~GAMESYSTEM()
{
}

void GAMESYSTEM::Run()
{
	while (bRunGame) {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			console.Input();
			keyboard.KeyEvent();
			Render();
		}
	}
}

void GAMESYSTEM::EndGame()
{
	cout << endl;
	bRunGame = false;
}

//	Other functions
//
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return gSystem.MessageHandler(hwnd, msg, wParam, lParam);
}

