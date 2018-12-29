#include "KEYBOARD.h"
#include "GAMESYSTEM.h"
#include <iostream>

KEYBOARD::KEYBOARD()
{
}


KEYBOARD::~KEYBOARD()
{
}
void KEYBOARD::Initialize()
{
	for (int i = 0; i < MAXKEYCOUNT; i++)
		keyboard[i] = false;
}
bool KEYBOARD::IsKeyDown(WPARAM _key)
{
	return keyboard[_key];
}
void KEYBOARD::KeyDown(WPARAM _key)
{
	if (_key < MAXKEYCOUNT)
		keyboard[_key] = true;
}
void KEYBOARD::KeyUp(WPARAM _key)
{
	if (_key < MAXKEYCOUNT)
		keyboard[_key] = false;
}
void KEYBOARD::KeyEvent()
{
	if (keyboard[VK_ESCAPE] == true)
		gSystem.EndGame();
}