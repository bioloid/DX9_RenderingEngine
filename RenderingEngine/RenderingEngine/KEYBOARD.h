#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__
#include <Windows.h>
#define MAXKEYCOUNT 128

class KEYBOARD
{
private:
	bool keyboard[MAXKEYCOUNT];
public:
	KEYBOARD();
	~KEYBOARD();
	void Initialize();
	bool IsKeyDown(WPARAM);
	void KeyDown(WPARAM);
	void KeyUp(WPARAM);
	void KeyEvent();
};

#endif