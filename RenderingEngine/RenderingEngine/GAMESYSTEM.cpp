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
template<class T> void Release_(T t);

//	Private Functions
//

void GAMESYSTEM::Release()
{
	console.SetFunction("Release");
	ShowWindow(hwnd, SW_SHOWMINIMIZED);
	for (auto ptr = texture.begin(); ptr != texture.end(); ptr++)
	{
		if (ptr->second != NULL)
			ptr->second->Release();
	}
	test.Release();
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
void GAMESYSTEM::Render()
{
	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);
	device->BeginScene();
	device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	device->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


//	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	DrawMSG();

	D3DXVECTOR4 cameraPosition(camera.position);
	cameraPosition.w = 1.0f;

	D3DXVECTOR4 lightPosition = { 0,10,0,1 };
	gSystem.shader->SetVector("gWorldLightPosition", &lightPosition);
	gSystem.shader->SetVector("gWorldCameraPosition", &cameraPosition);

	test.Render();

	device->EndScene();
	device->Present(0, 0, 0, 0);
}



void GAMESYSTEM::MSGPrint(int x, int y)
{
	FontBox.top = x;
	FontBox.left = y;
	font->DrawText(NULL, str, -1, &FontBox, DT_NOCLIP, D3DCOLOR_XRGB(0, 0, 0));
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
		avgfps.Initialize(AVG);
		instfps.Initialize(INST);
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

void GAMESYSTEM::D3DInitialize(bool _windowed, D3DDEVTYPE _deviceType)
{
	console.SetFunction("D3DInitialize");

	IDirect3D9* d3d9 = 0;
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d9)
	{
		console << con::error << con::func << "Direct3DCreate9() - failed" << con::endl;
		console << con::error << con::func << "critical error is detected" << con::endl;
		throw RUNTIME_ERROR(CRITICAL_DIRECTX_ERROR);
	}

	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, _deviceType, &caps);

	int vp = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	d3d9->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, _deviceType, D3DFMT_A8R8G8B8, false, D3DMULTISAMPLE_16_SAMPLES, NULL);

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = winSize.right;
	d3dpp.BackBufferHeight = winSize.bottom;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hwnd;
	d3dpp.Windowed = _windowed;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	//D3DPRESENT_INTERVAL_IMMEDIATE
	//D3DPRESENT_INTERVAL_DEFAULT
	if (FAILED(d3d9->CreateDevice(D3DADAPTER_DEFAULT, _deviceType, hwnd, vp, &d3dpp, &device)))
	{
		console << con::error << con::func << "CreateDevice() - D3DFMT_D24S8 failed" << con::endl;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16; // 16 - bit Buffer
		if (FAILED(d3d9->CreateDevice(D3DADAPTER_DEFAULT, _deviceType, hwnd, vp, &d3dpp, &device)))
		{
			d3d9->Release();
			console << con::error << con::func << "CreateDevice() - D3DFMT_D16 failed" << con::endl;
			console << con::error << con::func << "critical error is detected" << con::endl;
			throw RUNTIME_ERROR(CRITICAL_DIRECTX_ERROR);
		}
	}
	d3d9->Release();

	D3DXCreateFont(device, 15, 0, FW_DONTCARE, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "system", &font);

	//*********************************//
	//	Shader Load					   //
	//*********************************//
	try 
	{
		D3DEffectInitialize();
	}
	catch (RUNTIME_ERROR &e)
	{
		if (e.error == CRITICAL_DIRECTX_ERROR)
		{
			Release_<IDirect3DDevice9*>(device);
			console << con::info << con::func << "IDirect3DDevice9* Released" << con::endl;
			Release_<ID3DXFont*>(font);
			console << con::info << con::func << "ID3DXFont* Released" << con::endl;
			throw e;
		}
	}
	camera.Initialize();

	texture["defaultTexture"] = NULL;
	if (FAILED(D3DXCreateTextureFromFile
	(device, "Data\\Default\\Texture\\defaultTexture.png", &texture["defaultTexture"])))
	{
		console << con::error << con::func << "D3DXCreateTextureFromFile() - default error" << con::endl;
		console << con::error << con::func << "check default texture file" << con::endl;
		console << con::error << con::func << "path : Data\\Default\\Texture\\defaultTexture.png" << con::endl;
		Release_<IDirect3DDevice9*>(device);
		console << con::info << con::func << "IDirect3DDevice9* Released" << con::endl;
		Release_<ID3DXFont*>(font);
		console << con::info << con::func << "ID3DXFont* Released" << con::endl;
		Release_<LPD3DXEFFECT>(shader);
		console << con::info << con::func << "LPD3DXEFFECT Released" << con::endl;
		throw RUNTIME_ERROR(CRITICAL_DIRECTX_ERROR);
	}
	default_texture = &texture["defaultTexture"];
	shader->SetTexture("DiffuseMap_Tex", texture["defaultTexture"]);

	unsigned long obj_load_time = GetTickCount64();

	try 
	{
		D3DXMATRIXA16 test_;
		D3DXMatrixIdentity(&test_);
	//	test.Initialize("lowpolytree.obj", test_);
		test.Initialize("Tree.obj", test_);
	//	test.Initialize("wooden watch tower2.obj", test_);
	//	test.Initialize("MaleLow.obj", test_);
	//	test.Initialize("low-poly-mill.obj", test_);
	//	test.Initialize("IronMan.obj", test_);
	//	test.Initialize("v.obj", test_);
	}
	catch (RUNTIME_ERROR &e)
	{
		if (e.error == MODLE_LOAD_ERROR)
			test.bEnable = false;
	}
	console << con::info << con::func << "obj load time : " << (GetTickCount64() - obj_load_time) * 0.001f << con::endl;
	console.RestoreFunction();
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
		void *ack = error->GetBufferPointer();
		console << con::error << con::func << "D3DXCreateEffectFromFile() error" << con::endl;
		throw RUNTIME_ERROR(CRITICAL_DIRECTX_ERROR);
	}
	console << con::info << con::func << "shader Create succeed" << con::endl;
	console.RestoreFunction();
}

void GAMESYSTEM::D3DRelease()
{
	console.SetFunction("IN_D3DRelease");
	test.Release();
	Release_<IDirect3DDevice9*>(device);
	console << con::info << con::func << "IDirect3DDevice9* Released" << con::endl;
	Release_<ID3DXFont*>(font);
	console << con::info << con::func << "ID3DXFont* Released" << con::endl;
	Release_<LPD3DXEFFECT>(shader);
	console << con::info << con::func << "LPD3DXEFFECT Released" << con::endl;
	console.RestoreFunction();
}
#define T_ 0
#define F_ 1
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

template<class T> void Release_(T t)
{
	if (t != NULL)
	{
		t->Release();
		t = NULL;
	}
}

void GAMESYSTEM::DrawMSG()
{
	avgfps.Count();
	instfps.Count();
	ram.Update();
	cpu.Update();
	sprintf_s(str, sizeof(str) / sizeof(char), TEXT("RUN time : %.03lf"), time.getTime());
	MSGPrint(0, 0);
	sprintf_s(str, sizeof(str) / sizeof(char), TEXT("AVG FPS : %4.03lf"), avgfps.Get());
	MSGPrint(15, 0);
	sprintf_s(str, sizeof(str) / sizeof(char), TEXT("INST FPS : %4.03lf"), instfps.Get());
	MSGPrint(30, 0);
	sprintf_s(str, sizeof(str) / sizeof(char), TEXT("Total Virtual RAM : %dMB"), ram.GetTotalVirtualMEM() / (1024 * 1024));
	MSGPrint(45, 0);
	sprintf_s(str, sizeof(str) / sizeof(char), TEXT("Used Virtual RAM : %dMB"), ram.GetUsedVirtualMEM() / (1024 * 1024));
	MSGPrint(60, 0);
	sprintf_s(str, sizeof(str) / sizeof(char), TEXT("Total RAM : %dMB"), ram.GetTotalMEM() / (1024 * 1024));
	MSGPrint(75, 0);
	sprintf_s(str, sizeof(str) / sizeof(char), TEXT("Used RAM : %dMB"), ram.GetUsedMEM() / (1024 * 1024));
	MSGPrint(90, 0);
	sprintf_s(str, sizeof(str) / sizeof(char), TEXT("Proc RAM : %.03fMB"), (float)ram.GetProcUsedMEM() / (1024 * 1024));
	MSGPrint(105, 0);
	sprintf_s(str, sizeof(str) / sizeof(char), TEXT("Proc Virtual RAM : %.03fMB"), (float)ram.GetProcUsedVirtualMEM() / (1024 * 1024));
	MSGPrint(120, 0);
	sprintf_s(str, sizeof(str) / sizeof(char), TEXT("Total CPU : %d%%"), cpu.GetCpuUsage());
	MSGPrint(135, 0);
	sprintf_s(str, sizeof(str) / sizeof(char), TEXT("Proc CPU : %.1lf%%"), cpu.GetProcessUsage());
	MSGPrint(150, 0);
}