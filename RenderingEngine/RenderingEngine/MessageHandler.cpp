#include "GAMESYSTEM.h"

#pragma warning(disable:4477)
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

void GAMESYSTEM::MSGPrint(int x, int y)
{
	FontBox.top = x;
	FontBox.left = y;
	font->DrawText(NULL, str, -1, &FontBox, DT_NOCLIP, D3DCOLOR_XRGB(0, 0, 0));
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