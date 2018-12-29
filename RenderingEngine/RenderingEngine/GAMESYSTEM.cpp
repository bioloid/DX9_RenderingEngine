#include "GAMESYSTEM.h"
extern GAMESYSTEM gSystem;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
bool CtrlHandler(DWORD fdwCtrlType);



//	Private Functions
//

void GAMESYSTEM::IN_Initialize
(HINSTANCE _hInstance, HINSTANCE _prevInstance, LPSTR _cmdLine, int _showCmd)
{
	AllocConsole();
	SetConsoleTitle("Debug Console");
	consoleHND = FindWindow(NULL, "Debug Console");
	MoveWindow(consoleHND, windowSize.right + consoleSize.left, consoleSize.top, consoleSize.right, consoleSize.bottom, TRUE);
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	freopen("CONERR$", "w", stderr);

	if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE)){
		throw std::runtime_error("Console Handle Set Error");
	}
	std::cout << "INFO  : Initialize : Console Created" << std::endl;
	std::cout << "INFO  : Initialize : Initialize Check Passed" << std::endl;
	std::cout << "INFO  : Initialize : GAMESYSTEM Initialize started" << std::endl;

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
	std::cout << "INFO  : Initialize : RegisterClass succeed" << std::endl;

	hwnd = CreateWindow(pName, pName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, windowSize.right, windowSize.bottom, 0, 0, hInstance, 0);
	if (!hwnd) {
		throw std::runtime_error("CreateWindow failed");
	}
	std::cout << "INFO  : Initialize : CreateWindow succeed" << std::endl;

	MoveWindow(hwnd, 0, 0, windowSize.right, windowSize.bottom, TRUE);

	ShowWindow(hwnd, SW_SHOW);
//	SW_SHOW
//	SW_SHOWMAXIMIZED


	std::cout << "INFO  : Initialize : ShowWindow success" << std::endl;
	std::cout << "INFO  : Initialize : GAMESYSTEM Initialize ended" << std::endl;
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
	ShowWindow(hwnd, SW_SHOWMINIMIZED);
	pEndTime = pEndTime / 1000;
	while (pEndTime > 0)
	{
		std::cout << "INFO  : Release : End Program in " << pEndTime << " sec" << std::endl;
		Sleep(1000);
		pEndTime = pEndTime - 1;
	}
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
		std::cout << "ERROR : Initialize : " << e.what() << std::endl;
		std::cout << "INFO  : Initialize : End Program in " << pEndTime << "sec" << std::endl;
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
		GetClientRect(hwnd, &windowSize);
		break;

	case WM_MOUSEWHEEL:
		/*
		MouseWheel = mouse.IsMouseWheel(wParam);
		if (MouseWheel == UP) {

		}
		else if (MouseWheel == DOWN) {

		}

		break;
		*/
	case WM_MOUSEMOVE:
		/*
		if (mouse.x < LOWORD(lParam)) {
			gHandle->camera.rotation(MOUSE_RIGHT, LOWORD(lParam) - mouse.x);
		}
		else if (mouse.x > LOWORD(lParam)) {
			gHandle->camera.rotation(MOUSE_LEFT, mouse.x - LOWORD(lParam));
		}
		if (mouse.y < HIWORD(lParam)) {
			gHandle->camera.rotation(MOUSE_DOWN, HIWORD(lParam) - mouse.y);
		}
		else if (mouse.y > HIWORD(lParam)) {
			gHandle->camera.rotation(MOUSE_UP, mouse.y - HIWORD(lParam));
		}
		mouse.x = LOWORD(lParam);
		mouse.y = HIWORD(lParam);
		*/
		break;

	case WM_RBUTTONDOWN:
	//	mouse.MouseDown(MOUSE_RIGHT_BUTTON, LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_LBUTTONDOWN:
	//	mouse.MouseDown(MOUSE_LEFT_BUTTON, LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_RBUTTONUP:
	//	mouse.MouseUp(MOUSE_RIGHT_BUTTON);
		break;
	case WM_LBUTTONUP:
	//	mouse.MouseUp(MOUSE_LEFT_BUTTON);
		break;
	case WM_KEYDOWN:
	//	input.KeyDown(wParam);
		break;
	case WM_KEYUP:
	//	input.KeyUp(wParam);
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
/*
	case CTRL_BREAK_EVENT:
		std::cout << "CTRL_BREAK_EVENT" << std::endl;
	case CTRL_LOGOFF_EVENT:
		std::cout << "CTRL_LOGOFF_EVENT" << std::endl;
	case CTRL_SHUTDOWN_EVENT:
		std::cout << "CTRL_SHUTDOWN_EVENT" << std::endl;
*/
	default:
		return FALSE; 
	}
}
