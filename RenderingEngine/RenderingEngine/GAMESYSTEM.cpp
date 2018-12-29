//	Includes
//
#include "GAMESYSTEM.h"

#include <string>


//	Functions
//
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
bool CtrlHandler(DWORD fdwCtrlType);



//	Private Functions
//

void GAMESYSTEM::Release()
{
	console.setFunction("Release");
	ShowWindow(hwnd, SW_SHOWMINIMIZED);

	memory.Release();

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
	console << con::info << con::func << "ShowWindow succeed" << con::endl;

	keyboard.Initialize();
	mouse.Initialize();


	console << con::info << con::func << "GAMESYSTEM Initialize ended" << con::endl;
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
			keyboard.KeyEvent();			
		//	Test();
		//	Render();
		}
	}
}

void GAMESYSTEM::EndGame()
{
	runGame = false;
	Release();
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
