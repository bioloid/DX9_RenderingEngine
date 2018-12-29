#include "MOUSE.h"



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

int MOUSE::MouseMove(LPARAM lParam)
{
	int move = NONE;
	if (coor.x < LOWORD(lParam)) {
		move = RIGHT;
	}
	else if (coor.x > LOWORD(lParam)) {
		move = LEFT;
	}
	if (coor.y < HIWORD(lParam)) {
		move = DOWN;
	}
	else if (coor.y > HIWORD(lParam)) {
		move = UP;
	}
	coor.x = LOWORD(lParam);
	coor.y = HIWORD(lParam);
	return move;
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