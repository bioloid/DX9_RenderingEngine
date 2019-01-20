#ifndef _MOUSE_H__
#define _MOUSE_H__
#include <Windows.h>

#define MOUSE_RIGHT 1
#define MOUSE_LEFT  2
#define MOUSE_UP	3
#define MOUSE_DOWN  4
#define MOUSE_NONE  0
class MOUSE
{
private:
	POINT coor;
	bool L, R;
public:
	MOUSE();
	~MOUSE();
	void Initialize();
	void MouseMove(LPARAM);
	void MouseDown(WORD, LPARAM);
	void MouseUp(WORD, LPARAM);
	bool IsMouseDown(WORD);
	POINT MousePosition();

	int IsMouseWheel(WPARAM);
};

#endif