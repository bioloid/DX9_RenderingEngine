#include "GAMESYSTEM.h"

LRESULT CALLBACK GAMESYSTEM::MessageHandler
(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	#ifdef CONDEBUG
	console.SetFunction("MessageHandler");
	#endif // CONDEBUG

	switch (msg)
	{
	case WM_DESTROY:
		EndGame();
		break;

	case WM_CLOSE:
		EndGame();
		break;

	case WM_CREATE:
		break;

	case WM_PAINT:
		#ifdef CONDEBUG
			console << con::msg << con::func << "WM_PAINT" << con::endl;
		#endif // CONDEBUG
		break;

	case WM_MOUSEWHEEL:
		mouse.IsMouseWheel(wParam);

		#ifdef CONDEBUG
				console << con::msg << con::func << "WM_MOUSEWHEEL " << mouse.IsMouseWheel(wParam) << con::endl;
		#endif // CONDEBUG
		break;

	case WM_MOUSEMOVE:
		mouse.MouseMove(lParam);

		#ifdef CONDEBUG
				console << con::msg << con::func << "WM_MOUSEMOVE " << mouse.MousePosition() << con::endl;
		#endif // CONDEBUG
		break;

	case WM_RBUTTONDOWN:
		mouse.MouseDown(MOUSE_RIGHT, lParam);

		#ifdef CONDEBUG
				console << con::msg << con::func << "WM_RBUTTONDOWN " << mouse.MousePosition() << con::endl;
		#endif // CONDEBUG
		break;

	case WM_LBUTTONDOWN:
		mouse.MouseDown(MOUSE_LEFT, lParam);

		#ifdef CONDEBUG
				console << con::msg << con::func << "WM_LBUTTONDOWN " << mouse.MousePosition() << con::endl;
		#endif // CONDEBUG
		break;

	case WM_RBUTTONUP:
		mouse.MouseUp(MOUSE_RIGHT, lParam);

		#ifdef CONDEBUG
				console << con::msg << con::func << "WM_RBUTTONUP " << mouse.MousePosition() << con::endl;
		#endif // CONDEBUG
		break;

	case WM_LBUTTONUP:
		mouse.MouseUp(MOUSE_LEFT, lParam);

		#ifdef CONDEBUG
				console << con::msg << con::func << "WM_LBUTTONUP " << mouse.MousePosition() << con::endl;
		#endif // CONDEBUG
		break;

	case WM_KEYDOWN:
		gSystem.keyboard.KeyDown(wParam);

		#ifdef CONDEBUG
				console << con::msg << con::func << "WM_KEYDOWN" << " key : " << char(wParam) << con::endl;
		#endif // CONDEBUG
		break;

	case WM_KEYUP:		
		gSystem.keyboard.KeyUp(wParam);

		#ifdef CONDEBUG
				console << con::msg << con::func << "WM_KEYUP" << " key : " << (char)wParam << con::endl;
		#endif // CONDEBUG
		break;

	}
	#ifdef CONDEBUG
	console.RestoreFunction();
	#endif // CONDEBUG
	return DefWindowProc(hwnd, msg, wParam, lParam);
}