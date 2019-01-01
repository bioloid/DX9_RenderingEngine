#include "MOUSE.h"
#include "GAMESYSTEM.h"

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
	int moveXZ = NONE, moveY = NONE;
	int moveSize = 0;
	if (coor.x < LOWORD(lParam)) {
		moveXZ = RIGHT;
		moveSize = LOWORD(lParam) - coor.x;
	}
	else if (coor.x > LOWORD(lParam)) {
		moveXZ = LEFT;
		moveSize = coor.x - LOWORD(lParam);
	}
	if (coor.y < HIWORD(lParam)) {
		moveY = DOWN;
		moveSize = HIWORD(lParam) - coor.y;
	}
	else if (coor.y > HIWORD(lParam)) {
		moveY = UP;
		moveSize = coor.y - HIWORD(lParam);
	}
	gSystem.camera.rotation(moveXZ, moveY, moveSize);
	coor.x = LOWORD(lParam);
	coor.y = HIWORD(lParam);
}

void MOUSE::MouseDown(WORD data, LPARAM lParam)
{
	coor.x = LOWORD(lParam);
	coor.y = HIWORD(lParam);
	if (data == RIGHT) {
		R = true;
		return;
	}
	else if (data == LEFT) {
		L = true;
		return;
	}
}
void MOUSE::MouseUp(WORD data, LPARAM lParam)
{
	coor.x = LOWORD(lParam);
	coor.y = HIWORD(lParam);
	if (data == RIGHT) {
		R = false;
		return;
	}
	else if (data == LEFT) {
		L = false;
		return;
	}
}
bool MOUSE::IsMouseDown(WORD data)
{
	if (data == RIGHT) {
		return R;
	}
	else if (data == LEFT) {
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
		return UP;
	}
	else {
		return DOWN;
	}
}