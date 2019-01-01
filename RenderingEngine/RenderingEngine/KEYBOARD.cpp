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
	if (keyboard[int('Q')] || keyboard[int('q')])
		gSystem.camera.Move(KEY_Q);
	if (keyboard[int('E')] || keyboard[int('e')])
		gSystem.camera.Move(KEY_E);
	if (keyboard[int('W')] || keyboard[int('w')])
		gSystem.camera.Move(KEY_W);
	if (keyboard[int('A')] || keyboard[int('a')])
		gSystem.camera.Move(KEY_A);
	if (keyboard[int('S')] || keyboard[int('s')])
		gSystem.camera.Move(KEY_S);
	if (keyboard[int('D')] || keyboard[int('d')])
		gSystem.camera.Move(KEY_D);
}