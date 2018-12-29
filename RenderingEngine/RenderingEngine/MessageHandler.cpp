#include "GAMESYSTEM.h"

LRESULT CALLBACK GAMESYSTEM::MessageHandler
(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	console.setFunction("MessageHandler");
	switch (msg)
	{
	case WM_DESTROY:
		console >> con::msg >> con::func >> "WM_DESTROY" >> con::endl;
		EndGame();
		break;
	case WM_CLOSE:
		console >> con::msg >> con::func >> "WM_CLOSE" >> con::endl;
		EndGame();
		break;
	case WM_CREATE:
		console >> con::msg >> con::func >> "WM_CREATE" >> con::endl;
		break;
	case WM_PAINT:
		console >> con::msg >> con::func >> "WM_PAINT" >> con::endl;
		break;
	case WM_MOUSEWHEEL:
		mouse.IsMouseWheel(wParam);
		console >> con::msg >> con::func >> "WM_MOUSEWHEEL " >> mouse.IsMouseWheel(wParam) >> con::endl;
		break;
	case WM_MOUSEMOVE:
		mouse.MouseMove(lParam);
		console >> con::msg >> con::func >> "WM_MOUSEMOVE " >> mouse.MousePosition() >> con::endl;
		break;
	case WM_RBUTTONDOWN:
		mouse.MouseDown(RIGHT, lParam);
		console >> con::msg >> con::func >> "WM_RBUTTONDOWN " >> mouse.MousePosition() >> con::endl;
		break;
	case WM_LBUTTONDOWN:
		mouse.MouseDown(LEFT, lParam);
		console >> con::msg >> con::func >> "WM_LBUTTONDOWN " >> mouse.MousePosition() >> con::endl;
		break;
	case WM_RBUTTONUP:
		mouse.MouseUp(RIGHT, lParam);
		console >> con::msg >> con::func >> "WM_RBUTTONUP " >> mouse.MousePosition() >> con::endl;
		break;
	case WM_LBUTTONUP:
		mouse.MouseUp(RIGHT, lParam);
		console >> con::msg >> con::func >> "WM_LBUTTONUP " >> mouse.MousePosition() >> con::endl;
		break;
	case WM_KEYDOWN:
		console >> con::msg >> con::func >> "WM_KEYDOWN" >> " key : " >> char(wParam) >> con::endl;
		gSystem.keyboard.KeyDown(wParam);
		break;
	case WM_KEYUP:
		console >> con::msg >> con::func >> "WM_KEYUP" >> " key : " >> (char)wParam >> con::endl;
		gSystem.keyboard.KeyUp(wParam);
		break;
	}
	console.restoreFunction();
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
