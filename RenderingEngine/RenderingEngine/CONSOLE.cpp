#include "CONSOLE.h"
#include "GAMESYSTEM.h"
#include <iostream>

extern GAMESYSTEM gSystem;

namespace con
{
	ENDL_ endl;
	FUNC_ func;
	INFO_ info;
	ERROR_ error;

CONSOLE::CONSOLE()
{
}


CONSOLE::~CONSOLE()
{
}

void CONSOLE::setFunction(string _function)
{
	function = _function;
}
void CONSOLE::print()
{
	if (gSystem.consoleDebug == false)
		return;

	if (function.length() <= 0)
		return;
	else
	{
		if (function.length() > PRINTSIZE)
			cout << " :" << function << ": ";
		else
		{
			int front = PRINTSIZE - function.length();
			int back = front;
			front = front / 2;
			back = back - front;
			cout << " :";
			for (int i = 0; i < front; i++)
				cout << " ";
			cout << function;
			for (int i = 0; i < back; i++)
				cout << " ";
			cout << ": ";
		}
	}
}

CONSOLE& CONSOLE::operator<<(string _data)
{
	if (gSystem.consoleDebug == true)
		cout << _data;
	return *this;
}

CONSOLE& CONSOLE::operator<<(int _data)
{
	if (gSystem.consoleDebug == true)
		cout << _data;
	return *this;

}

CONSOLE& CONSOLE::operator<<(float _data)
{
	if (gSystem.consoleDebug == true)
		cout << _data;
	return *this;
}

CONSOLE& CONSOLE::operator<<(double _data)
{
	if (gSystem.consoleDebug == true)
		cout << _data;
	return *this;
}
CONSOLE& CONSOLE::operator<<(ENDL_ _data)
{
	if (gSystem.consoleDebug == true)
		cout << std::endl;
	return *this;
}
CONSOLE& CONSOLE::operator<<(FUNC_ _data)
{
	if (gSystem.consoleDebug == true)
		print();
	return *this;
}
CONSOLE& CONSOLE::operator<<(INFO_ _data)
{
	if (gSystem.consoleDebug == true)
		cout << " INFO ";
	return *this;
}
CONSOLE& CONSOLE::operator<<(ERROR_ _data)
{
	if (gSystem.consoleDebug == true)
		cout << " ERROR";
	return *this;
}


CONSOLE& CONSOLE::operator>>(string _data)
{
	if (gSystem.consoleDebug == true)
		cout << _data;
	return *this;
}

CONSOLE& CONSOLE::operator>>(int _data)
{
	if (gSystem.consoleDebug == true)
		cout << _data;
	return *this;
}

CONSOLE& CONSOLE::operator>>(float _data)
{
	if (gSystem.consoleDebug == true)
		cout << _data;
	return *this;
}

CONSOLE& CONSOLE::operator>>(double _data)
{
	if (gSystem.consoleDebug == true)
		cout << _data;
	return *this;
}
}