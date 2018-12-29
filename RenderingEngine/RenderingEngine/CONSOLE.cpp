#include "CONSOLE.h"
#include "GAMESYSTEM.h"

#include <iostream>


namespace con
{
	ENDL_	endl;
	FUNC_	func;
	INFO_	info;
	ERROR_	error;
	MSG_	msg;
}
CONSOLE::CONSOLE()
{
}


CONSOLE::~CONSOLE()
{
}

void CONSOLE::setFunction(string _function)
{
	function_before = function;
	function = _function;
}
void CONSOLE::restoreFunction()
{
	function = function_before;
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
			int front = PRINTSIZE - (int)function.length();
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
CONSOLE& CONSOLE::operator<<(con::ENDL_ _data)
{
	if (gSystem.consoleDebug == true)
		cout << std::endl;
	return *this;
}
CONSOLE& CONSOLE::operator<<(con::FUNC_ _data)
{
	if (gSystem.consoleDebug == true)
		print();
	return *this;
}
CONSOLE& CONSOLE::operator<<(con::INFO_ _data)
{
	if (gSystem.consoleDebug == true)
		cout << ": INFO ";
	return *this;
}
CONSOLE& CONSOLE::operator<<(con::ERROR_ _data)
{
	if (gSystem.consoleDebug == true)
		cout << ": ERROR";
	return *this;
}
CONSOLE& CONSOLE::operator<<(con::MSG_ _data)
{
	if (gSystem.consoleDebug == true)
		cout << ": MSG  ";
	return *this;
}


CONSOLE& CONSOLE::operator>>(string _data)
{
	if (gSystem.debug == true)
		cout << _data;
	return *this;
}
CONSOLE& CONSOLE::operator>>(int _data)
{
	if (gSystem.debug == true)
		cout << _data;
	return *this;
}
CONSOLE& CONSOLE::operator>>(float _data)
{
	if (gSystem.debug == true)
		cout << _data;
	return *this;
}
CONSOLE& CONSOLE::operator>>(double _data)
{
	if (gSystem.debug == true)
		cout << _data;
	return *this;
}
CONSOLE& CONSOLE::operator>>(con::ENDL_ _data)
{
	if (gSystem.debug == true)
		cout << std::endl;
	return *this;
}
CONSOLE& CONSOLE::operator>>(con::FUNC_ _data)
{
	if (gSystem.debug == true)
		print();
	return *this;
}
CONSOLE& CONSOLE::operator>>(con::INFO_ _data)
{
	if (gSystem.debug == true)
		cout << ": INFO ";
	return *this;
}
CONSOLE& CONSOLE::operator>>(con::ERROR_ _data)
{
	if (gSystem.debug == true)
		cout << ": ERROR";
	return *this;
}
CONSOLE& CONSOLE::operator>>(con::MSG_ _data)
{
	if (gSystem.debug == true)
		cout << "D: MSG ";
	return *this;
}
CONSOLE& CONSOLE::operator>>(POINT _data)
{
	if (gSystem.debug == true)
		cout << "x : " << _data.x << " y : " << _data.y;
	return *this;
}
