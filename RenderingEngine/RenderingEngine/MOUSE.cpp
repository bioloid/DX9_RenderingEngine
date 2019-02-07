#include "MOUSE.h"
#include "GAMESYSTEM.h"
#pragma warning (disable:4715)
MOUSE::MOUSE()
{
}

MOUSE::~MOUSE()
{
}

void MOUSE::Initialize()
{
	coor.x = 0; coor.y = 0;
	L = false; R = false;
}

void MOUSE::MouseMove(LPARAM lParam)
{
	gSystem.camera.MouseMove(LOWORD(lParam) - coor.x, HIWORD(lParam) - coor.y);
	coor.x = LOWORD(lParam);
	coor.y = HIWORD(lParam);
}

void MOUSE::MouseDown(WORD data, LPARAM lParam)
{
	coor.x = LOWORD(lParam);
	coor.y = HIWORD(lParam);
	if (data == MOUSE_RIGHT) {
		R = true;
		return;
	}
	else if (data == MOUSE_LEFT) {
		L = true;
		return;
	}
}
void MOUSE::MouseUp(WORD data, LPARAM lParam)
{
	coor.x = LOWORD(lParam);
	coor.y = HIWORD(lParam);
	if (data == MOUSE_RIGHT) {
		R = false;
		return;
	}
	else if (data == MOUSE_LEFT) {
		L = false;
		return;
	}
}
bool MOUSE::IsMouseDown(WORD data)
{
	if (data == MOUSE_RIGHT) {
		return R;
	}
	else if (data == MOUSE_LEFT) {
		return L;
	}
}
POINT MOUSE::MousePosition()
{
	return coor;
}

int MOUSE::IsMouseWheel(WPARAM data)
{
	if ((SHORT)HIWORD(data) > 0) {
		return MOUSE_UP;
	}
	else {
		return MOUSE_DOWN;
	}
}