//	Includes
//
#include "GAMESYSTEM.h"

#include <string>


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

	memory.Release();
	cpu.Release();
	IN_D3DRelease();
	pEndTime = pEndTime / 1000;
	while (pEndTime > 0)
	{
		console << con::info << con::func << "End Program in " << pEndTime << "sec\n";
		Sleep(1000);
		pEndTime = pEndTime - 1;
	}
}
void GAMESYSTEM::Render()
{
	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);
	device->BeginScene();


	DrawMSG();


	device->EndScene();
	device->Present(0, 0, 0, 0);
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
void GAMESYSTEM::MSGPrint(int x, int y)
{
	FontBox.top = x;
	FontBox.left = y;
	font->DrawText(NULL, str, -1, &FontBox, DT_NOCLIP, D3DCOLOR_XRGB(0, 0, 0));
}
void GAMESYSTEM::IN_Initialize
(HINSTANCE _hInstance, HINSTANCE _prevInstance, LPSTR _cmdLine, int _showCmd)
{
//	Command argc, argv
//	Filename -c -d
//
	if (__argc >= 2)
	{
		if (__argv[1] == "-c")
			consoleDebug = true;
		if (__argc >= 3)
		{
			if (__argv[2] == "-d")
				debug = true;
		}
	}

//	Class Initialization
//
	console.Initialize();
	keyboard.Initialize();
	mouse.Initialize();
	time.Initialize();
	avgfps.Initialize(AVG);
	instfps.Initialize(INST);
	cpu.Initialize();
	ram.Initialize();

	console.SetFunction("Initialize");
	console << con::info << con::func << "CONSOLE started" << con::endl;
	console << con::info << con::func << "Initialize Check Passed" << con::endl;
	console << con::info << con::func << "GAMESYSTEM Initialize started" << con::endl;


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
	wc.lpszClassName = pName;
	if (!RegisterClass(&wc)) {
		throw std::runtime_error("RegisterClass failed");
	}
	console << con::info << con::func << "RegisterClass succeed" << con::endl;

	hwnd = CreateWindow(pName, pName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, windowSize.right, windowSize.bottom, 0, 0, hInstance, 0);
	if (!hwnd) {
		throw std::runtime_error("CreateWindow failed");
	}
	console << con::info << con::func << "CreateWindow succeed" << con::endl;

	MoveWindow(hwnd, 0, 0, windowSize.right, windowSize.bottom, TRUE);

	ShowWindow(hwnd, SW_SHOW);
//	SW_SHOW
//	SW_SHOWMAXIMIZED
	console << con::info << con::func << "ShowWindow succeed" << con::endl;

	
	console << con::info << con::func << "GAMESYSTEM Initialize ended" << con::endl;
}

void GAMESYSTEM::IN_D3DInitialize(bool _windowed, D3DDEVTYPE _deviceType)
{
	console.SetFunction("D3DInitialize");
	console << con::info << con::func << "Initialize Check Passed" << con::endl;
	console << con::info << con::func << "GAMESYSTEM D3DInitialize started" << con::endl;

	IDirect3D9* d3d9 = 0;
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d9)
		throw runtime_error("d3d9 Direct3DCreate9 failed");
	console << con::info << con::func << "d3d9 Direct3DCreate9 succeed" << con::endl;


	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, _deviceType, &caps);
	console << con::info << con::func << "d3d9 GetDeviceCaps succeed" << con::endl;

	int vp = 0;

	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	d3d9->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, _deviceType, D3DFMT_A8R8G8B8, false, D3DMULTISAMPLE_16_SAMPLES, NULL);


	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = windowSize.right;
	d3dpp.BackBufferHeight = windowSize.bottom;
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
		console << con::error << con::func << "hr CreateDevice Failed" << con::endl;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16; // 16 - bit Buffer
		if (FAILED(d3d9->CreateDevice(D3DADAPTER_DEFAULT, _deviceType, hwnd, vp, &d3dpp, &device)))
		{
			d3d9->Release();
			throw runtime_error("hr 16-bit CreateDevice Failed");
		}
	}
	d3d9->Release();
	console << con::info << con::func << "device create succeed" << con::endl;

	D3DXCreateFont(device, 15, 0, FW_DONTCARE, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "system", &font);
	console << con::info << con::func << "font create succeed" << con::endl;
	try {
		IN_D3DEffectInitialize();
	}
	catch (std::exception &e){
		console << con::info << con::func << e.what() << con::endl;
		throw e;
	}


	console << con::info << con::func << "GAMESYSTEM D3DInitialize ended" << con::endl;
	console.RestoreFunction();
}
void GAMESYSTEM::IN_D3DEffectInitialize()
{
	console.SetFunction("D3DEffectInitialize");

	LPD3DXBUFFER error = NULL;
	DWORD shaderflags = 0;
#if _DEBUG
	shaderflags |= D3DXSHADER_DEBUG;
#endif
	D3DXCreateEffectFromFile(device, "shader.fx", NULL, NULL, shaderflags, NULL, &shader, &error);
	if (!shader && error)
	{
		int size_ = error->GetBufferSize();
		void *ack = error->GetBufferPointer();
		if (ack)
			delete[] str;

		throw std::runtime_error("D3DXCreateEffectFromFile Error");
	}
	console << con::info << con::func << "shader Create succeed" << con::endl;
	console.RestoreFunction();
}

void GAMESYSTEM::IN_D3DRelease()
{
	console.SetFunction("IN_D3DRelease");


	Release_<IDirect3DDevice9*>(device);
	console << con::info << con::func << "IDirect3DDevice9* Released" << con::endl;
	Release_<ID3DXFont*>(font);
	console << con::info << con::func << "ID3DXFont* Released" << con::endl;
	Release_<LPD3DXEFFECT>(shader);
	console << con::info << con::func << "LPD3DXEFFECT Released" << con::endl;


	console.RestoreFunction();
}


void GAMESYSTEM::Test()
{
	static int a = 0, b = 1;
	if (a == 0)
		cout << a << " : " << memory.Allocate(sizeof(int)*b, 0) << endl;
	else if (a == 1)
		cout << a << " : " << memory.Allocate(sizeof(char)*b, 0) << endl;
	else if (a == 2)
		cout << a << " : " << memory.Allocate(sizeof(double)*b, 0) << endl;
	else if (a == 3)
		cout << a << " : " << memory.Allocate(sizeof(float)*b, 0) << endl;
	else if (a == 4)
	{
		memory.Clear(0);
		cout << a << " : Clear" << endl;
	}
	else if (a == 5)
		cout << a << " : " << memory.Allocate(sizeof(int)*b, 1) << endl;
	else if (a == 6)
		cout << a << " : " << memory.Allocate(sizeof(char)*b, 1) << endl;
	else if (a == 7)
		cout << a << " : " << memory.Allocate(sizeof(double)*b, 1) << endl;
	else if (a == 8)
		cout << a << " : " << memory.Allocate(sizeof(float)*b, 1) << endl;
	else if (a == 9)
	{
		memory.Clear(1);
		cout << a << " : Clear" << endl;
	}
	a++;
	b++;
	b = b % 100;
	a = a % 10;
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
	while (runGame){
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			console.input();
			keyboard.KeyEvent();
			Render();
		//	Test();
		//	Render();
		}
	}
}

void GAMESYSTEM::EndGame()
{
	runGame = false;
}



void GAMESYSTEM::Initialize
(HINSTANCE _hInstance, HINSTANCE _prevInstance, LPSTR _cmdLine, int _showCmd, int _check)
{
	try {
		if (_check != 8)
			throw std::runtime_error("Initialize Check Error");
		IN_Initialize(_hInstance, _prevInstance, _cmdLine, _showCmd);
	}
	catch (const std::exception &e) {
		console << con::error << con::func << e.what() << "\n";
		console << con::info << con::func << "End Program in " << pEndTime << " sec\n";
		Sleep(pEndTime);
		throw;
	}
}

void GAMESYSTEM::D3DInitialize(bool _windowed, D3DDEVTYPE _deviceType, int _check)
{
	try {
		if (_check != 8)
			throw std::runtime_error("Initialize Check Error");
		IN_D3DInitialize(_windowed, _deviceType);
	}
	catch (const std::exception &e) {
		console << con::error << con::func << e.what() << "\n";
		console << con::info << con::func << "End Program in " << pEndTime << " sec\n";
		Sleep(pEndTime);
		throw;
	}
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

