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
	Release_<LPD3DXEFFECT>(shader);
	console << con::info << con::func << "LPD3DXEFFECT shader Released" << con::endl;
	Release_<LPD3DXEFFECT>(shadow);
	console << con::info << con::func << "LPD3DXEFFECT shadow Released" << con::endl;
	Release_<LPDIRECT3DTEXTURE9>(gpShadowRenderTarget);
	Release_<LPDIRECT3DSURFACE9>(gpShadowDepthStencil);
	Release_<LPDIRECT3DTEXTURE9>(gpRenderTarget);
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

	try
	{
		console.Initialize();
		keyboard.Initialize();
		mouse.Initialize();
		time.Initialize();
		cpu.Initialize();
		ram.Initialize();
	}
	//	class error handling
	//
	catch (RUNTIME_ERROR &e)
	{
		if (e.error == CRITICAL_CLASS_ERROR)
			throw e;
	}

	//	read program info
	//
	int build_number;
	string version;
	char build_number_tmp[16];

	ifstream version_read;
	ofstream version_write;

	version_read.open("ver.txt");
	if (version_read.fail())
	{
		console << con::error << con::func << "there is no ver.txt" << con::endl;
		console << con::info << con::func << "default ver.txt file is created" << con::endl;

		version_read.clear();
		version_read.close();
		version_write.open("ver.txt");
		version_write << DEFAULTVER << '\n' << DEFAULTBUILDNUMBER;
		version_write.close();
		version = DEFAULTVER;
		build_number = DEFAULTBUILDNUMBER;
	}

	else
	{
		version_read >> version >> build_number;
		build_number++;
		version_read.close();
		version_write.open("ver.txt", ios::trunc);
		version_write << version << '\n' << build_number;
		version_write.close();
	}

	pName = "Rendering Engine " + version + " build " + string(itoa(build_number, build_number_tmp, 10));
	console << con::info << con::func << "program info is ready" << con::endl;

	try
	{
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
	catch (RUNTIME_ERROR &e)
	{
		if (e.error == CRITICAL_WINDOW_ERROR)
		{
			console << con::error << con::func << e.info() << con::endl;
			memory.Release();
			cpu.Release();
			pEndWaitTime = pEndWaitTime / 1000;
			while (pEndWaitTime > 0)
			{
				console << con::info << con::func << "End Program in " << pEndWaitTime << "sec\n";
				Sleep(1000);
				pEndWaitTime--;
			}
			console.Release();
			throw e;
		}
	}
}

void GAMESYSTEM::D3DEffectInitialize()
{
	console.SetFunction("D3DEffectInitialize");

	LPD3DXBUFFER error = NULL;
	DWORD shaderflags = 0;

#if _DEBUG
	shaderflags |= D3DXSHADER_DEBUG;
#endif

	D3DXCreateEffectFromFile(device, "shader\\shader.fx", NULL, NULL, shaderflags, NULL, &shader, &error);
	if (!shader && error)
	{
		int size_ = error->GetBufferSize();
		char *ack = (char*)error->GetBufferPointer();
		cout << ack << endl;
		getchar();
		console << con::error << con::func << "D3DXCreateEffectFromFile() error" << con::endl;
		throw RUNTIME_ERROR(CRITICAL_DIRECTX_ERROR);
	}
	D3DXCreateEffectFromFile(device, "shader\\shadow.fx", NULL, NULL, shaderflags, NULL, &shadow, &error);
	if (!shader && error)
	{
		int size_ = error->GetBufferSize();
		void *ack = error->GetBufferPointer();
		console << con::error << con::func << "D3DXCreateEffectFromFile() error" << con::endl;
		throw RUNTIME_ERROR(CRITICAL_DIRECTX_ERROR);
	}
	console << con::info << con::func << "shader Create succeed" << con::endl;
	console.RestoreFunction();
}



void GAMESYSTEM::Test()
{
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

