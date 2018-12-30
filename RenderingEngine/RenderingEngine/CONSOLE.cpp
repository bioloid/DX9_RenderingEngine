#include "CONSOLE.h"
#include "GAMESYSTEM.h"
#include <iostream>
#include <conio.h> //_getch(), _kbhit()

extern GAMESYSTEM gSystem;

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


//	Console Event Handler
//
bool CtrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
	case CTRL_C_EVENT:
		return FALSE;
	case CTRL_CLOSE_EVENT:
		gSystem.runGame = false;
		Sleep(1000 * 1000);
		return FALSE;

	default:
		return FALSE;
	}
}

void CONSOLE::input()
{
	if (_kbhit())
	{
		char data = _getch();
		if (data == -32) // πÊ«‚≈∞
		{
			list<string>::iterator tmp;
			data = _getch();
			if (data == 72) // up
			{
				if (history.size() > 1)
				{
					tmp = hisptr;
					if (++tmp != history.end())
					{
						hisptr++;
						for (int i = 0; i < userinput.length(); i++)
							cout << "\b" << " " << "\b";
						userinput = *hisptr;
						cout << userinput;
					}
				}
			}
			else if (data == 80) // down
			{
				if (history.size() > 1)
				{
					if (hisptr != history.begin())
					{
						tmp = hisptr;
						if (--tmp != history.begin())
						{
							hisptr--;
							for (int i = 0; i < userinput.length(); i++)
								cout << "\b" << " " << "\b";
							userinput = *hisptr;
							cout << userinput;
						}
					}
				}
			}
			else if (data == 75) { // left
			}
			else if (data == 77) { // right
			}
		}
		else if (data == 13) // enter
		{
			if (!userinput.empty())
			{
				cout << endl;
				userinput += '\0';
				outinput.clear();
				int start, end, length = userinput.length();
				for (start = end = 0; start <= length && end <= length; end++)
				{
					if (userinput[end] == ' ' || end == length)
					{
						string tmp;
						for (int i = start; i < end; i++)
							tmp += userinput[i];
						outinput.push_back(tmp);
						start = end + 1;
					}
				}
				history.pop_front();
				history.push_front(userinput);
				history.push_front(" ");
				hisptr = history.begin();
				userinput.clear();
				Check();
			}
		}
		else if (data == 9) // tab
		{
			if (!userinput.empty())
			{
				int spacebar = userinput.rfind(" ");
				int pos;
			//	cout << spacebar;

				char data[4];
				data[3] = '\0';
				for (auto ptr = tabData.begin(); ptr != tabData.end(); ptr++)
				{
					for (int i = 0; i < 3; i++)
						data[i] = (*ptr)[i];
					
					pos = userinput.rfind(data);
					if (pos > spacebar)
					{
						int len = userinput.length();
						for (int i = 0; i < len - spacebar - 1; i++)
						{
							cout << "\b" << " " << "\b";
							userinput.pop_back();
						}
						cout << *ptr;
						userinput += *ptr;
						break;
					}

				}
			}
		}
		else if (data == 8) // backspace
		{
			if (userinput.length() > 0) {
				userinput.pop_back();
				cout << "\b" << " " << "\b";
			}
		}
		else				// normal input, space bar
		{
			userinput += data;
			cout << data;
		}
	}
}
void CONSOLE::Check()
{
	cout << "history" << endl;
	for (auto ptr = history.begin(); ptr != history.end(); ptr++)
		cout << *ptr << endl;
	cout << "outinput" << endl;
	for (auto ptr = outinput.begin(); ptr != outinput.end(); ptr++)
		cout << *ptr << endl;
}


void CONSOLE::Initialize()
{
	tabData.push_back("camera");
	tabData.push_back("point");
	tabData.push_back("solid");
	tabData.push_back("wireframe");
	tabData.push_back("cullmode");
	tabData.push_back("coordinate");
	tabData.push_back("angle");
	tabData.push_back("distence");
	tabData.push_back("speed");
	tabData.push_back("minimum");
	tabData.push_back("maximum");
	history.push_front(" ");
	hisptr = history.begin();

	if (gSystem.consoleDebug == true)
	{
		AllocConsole();
		SetConsoleTitle("Debug Console");
		consoleHND = FindWindow(NULL, "Debug Console");
		MoveWindow(consoleHND, gSystem.windowSize.right + gSystem.consoleSize.left, gSystem.consoleSize.top, gSystem.consoleSize.right, gSystem.consoleSize.bottom, TRUE);
		freopen("CONOUT$", "w", stdout);
		freopen("CONIN$", "r", stdin);
		freopen("CONERR$", "w", stderr);

		if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE)) {
			throw std::runtime_error("Console Handle Set Error");
		}
	}
}
void CONSOLE::Release()
{
	if (gSystem.consoleDebug == true)
		FreeConsole();
}

void CONSOLE::SetFunction(string _function)
{
	function_before = function;
	function = _function;
}
void CONSOLE::RestoreFunction()
{
	function = function_before;
}

void CONSOLE::Print()
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
		Print();
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
		Print();
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
