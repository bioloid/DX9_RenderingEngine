#ifndef _MOUSE_H__
#define _MOUSE_H__
#include <Windows.h>

#define RIGHT 1
#define LEFT  2
#define UP	  3
#define DOWN  4
#define NONE  0
class MOUSE
{
private:
	POINT coor;
	bool L, R;
public:
	MOUSE();
	~MOUSE();
	void Initialize();
	int MouseMove(LPARAM);
	void MouseDown(WORD, LPARAM);
	void MouseUp(WORD, LPARAM);
	bool IsMouseDown(WORD);
	POINT MousePosition();

	int IsMouseWheel(WPARAM);
};

#endif