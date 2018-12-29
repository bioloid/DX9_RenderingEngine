//	Includes
//
#include "GAMESYSTEM.h"
#include <string>

//	extern Variables
//
extern GAMESYSTEM gSystem;


//	Functions
//
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
bool CtrlHandler(DWORD fdwCtrlType);



//	Private Functions
//

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


//	Console Allocation
//	consoleHND : Console Handle
//
	if (consoleDebug == true)
	{
		console.setFunction("Initialize");

		AllocConsole();
		SetConsoleTitle("Debug Console");
		consoleHND = FindWindow(NULL, "Debug Console");
		MoveWindow(consoleHND, windowSize.right + consoleSize.left, consoleSize.top, consoleSize.right, consoleSize.bottom, TRUE);
		freopen("CONOUT$", "w", stdout);
		freopen("CONIN$", "r", stdin);
		freopen("CONERR$", "w", stderr);

		if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE)) {
			throw std::runtime_error("Console Handle Set Error");
		}
		console << con::info << con::func << "Console Created" << con::endl;
	}
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
	console << con::info << con::func << "ShowWindow success" << con::endl;




	console << con::info << con::func << "GAMESYSTEM Initialize ended" << con::endl;
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
		//	Render();
		}
	}
}

void GAMESYSTEM::Release()
{
	console.setFunction("Release");
	ShowWindow(hwnd, SW_SHOWMINIMIZED);
	pEndTime = pEndTime / 1000;
	while (pEndTime > 0)
	{
		console << con::info << con::func << "End Program in " << pEndTime << "sec\n";
		Sleep(1000);
		pEndTime = pEndTime - 1;
	}
	if (consoleDebug == true)
		FreeConsole();
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


LRESULT CALLBACK GAMESYSTEM::MessageHandler
(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		runGame = false;
		break;
	case WM_CLOSE:
		runGame = false;
		break;
	case WM_CREATE:
		break;
	case WM_PAINT:
		break;
	case WM_MOUSEWHEEL:
		break;
	case WM_MOUSEMOVE:
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_RBUTTONUP:
		break;
	case WM_LBUTTONUP:
		break;
	case WM_KEYDOWN:
		break;
	case WM_KEYUP:
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

//	Other functions
//
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{		
	return gSystem.MessageHandler(hwnd, msg, wParam, lParam);
}

//	Console Event Handler
//
bool CtrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
	case CTRL_C_EVENT:
		return FALSE;
	case CTRL_CLOSE_EVENT:
		gSystem.runGame = false;
		Sleep(1000*1000);
		return FALSE;

	default:
		return FALSE; 
	}
}
