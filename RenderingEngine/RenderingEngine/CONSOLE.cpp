#include "CONSOLE.h"
#include "GAMESYSTEM.h"
#include <iostream>
#include <conio.h> //_getch(), _kbhit()
#include <fstream>
#include <stdio.h>

#include "CheckDataDefine.h"


#pragma warning(disable:4267)
#pragma warning(disable:4244)

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
		gSystem.bRunGame = false;
		Sleep(1000 * 1000);
		return FALSE;

	default:
		return FALSE;
	}
}
bool CONSOLE::IsStr(string _data)
{
	for (auto ptr = _data.begin(); ptr != _data.end(); ptr++)
	{
		if (*ptr < 48 && *ptr > 57)
			return true;
	}
	return false;
}

void CONSOLE::Input()
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
				while (userinput.back() == ' ')
					userinput.pop_back();

				userinput += '\0';
				outinput.clear();
				int start, end, length = userinput.length();
				for (start = end = 0; start < length && end < length; end++)
				{
					if (userinput[end] == ' ' || userinput[end] == '\0')
					{
						string tmp;
						for (int i = start; i < end; i++)
							tmp += userinput[i];
						outinput.push_back(tmp);
						start = end + 1;
					}
				}
				userinput.pop_back();
				history.pop_front();
				history.push_front(userinput);
				history.push_front(" ");
				hisptr = history.begin();
				userinput.clear();
				Check();
				if (gSystem.bRunGame == true)
					cout << ">> ";
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
						cout << *ptr << " ";
						userinput += *ptr;
						userinput += " ";
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
		else if (data == 32)
		{
			if (userinput.back() != ' ')
			{
				userinput += data;
				cout << data;
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
#ifdef CONDEBUG
	cout << "User Input Info" << endl;
	cout << "history" << endl;
	for (auto ptr = history.begin(); ptr != history.end(); ptr++)
		cout << *ptr << ":" << endl;

	cout << "outinput" << endl;
	for (auto ptr = outinput.begin(); ptr != outinput.end(); ptr++)
		cout << *ptr << ":" << endl;

#endif // CONDEBUG


	int count = outinput.size();
	auto ptr = outinput.begin();
	if (count == 1)
	{
		if (*ptr == "ver")
		{
			ptr = checkData["ver"].begin();
			cout << endl << *++ptr << " v" << RENDERING_VER << endl;
		}
		else if (*ptr == "camera")
		{
			auto printData = checkData["camera"].begin();
			cout << endl << *printData << " x : " << gSystem.camera.position.x;
			cout << endl << *printData << " y : " << gSystem.camera.position.y;
			cout << endl << *printData << " z : " << gSystem.camera.position.z;
			cout << endl << *++printData << " xyz : " << gSystem.camera.speed.s;
			cout << endl << *printData << " theta : " << gSystem.camera.speed.th;
			cout << endl << *printData << " pi : " << gSystem.camera.speed.pi;
			cout << endl << *++printData << " theta : " << gSystem.camera.angle[0] * 57.296575f;
			cout << endl << *printData << " pi : " << gSystem.camera.angle[1] * 57.296575f;
			cout << endl << *++printData << " MAX : " << gSystem.camera.far_;
			cout << endl << *printData << " MIN : " << gSystem.camera.near_;
			cout << endl << *++printData << gSystem.camera.fov * 57.296575f << endl;
		}
		else if (*ptr == "exit")
		{
			gSystem.EndGame();
		}
		else
		{
			cout << endl << checkData["error"].front() << endl;
		}
	}
	else if (count == 2)
	{
		if (*ptr == "ver")
		{
			if (*++ptr == "-c")
				cout << endl << *++(++checkData["ver"].begin()) << " v" << CONSOLE_VER << endl;
			else if (*ptr == "-h")
				cout << endl << checkData["ver"].front() << endl;
			else
				cout << endl << checkData["error"].front() << endl;
		}
		else if (*ptr == "camera")
		{
			if (*++ptr == "coordinate")
			{
				string printData = checkData["camera"].front();
				cout << endl << printData << " x : " << gSystem.camera.position.x;
				cout << endl << printData << " y : " << gSystem.camera.position.y;
				cout << endl << printData << " z : " << gSystem.camera.position.z << endl;
			}
			else if (*ptr == "speed")
			{
				string printData = *++checkData["camera"].begin();
				cout << endl << printData << " xyz : " << gSystem.camera.speed.s;
				cout << endl << printData << " theta : " << gSystem.camera.speed.th;
				cout << endl << printData << " pi : " << gSystem.camera.speed.pi << endl;
			}
			else if (*ptr == "angle")
			{
				string printData = *++++checkData["camera"].begin();
				cout << endl << printData << " theta : " << gSystem.camera.angle[0] * 57.296575f;
				cout << endl << printData << " pi : " << gSystem.camera.angle[1] * 57.296575f << endl;
			}
			else if (*ptr == "distance")
			{
				string printData = *++++++checkData["camera"].begin();
				cout << endl << printData << " MAX : " << gSystem.camera.far_;
				cout << endl << printData << " MIN : " << gSystem.camera.near_ << endl;
			}
			else if (*ptr == "fov")
			{
				string printData = *++++++++checkData["camera"].begin();
				cout << endl << printData << gSystem.camera.fov * 57.296575f << endl;
			}
			else if (*ptr == "-h")
			{
				string printData = checkData["camera"].back();
				cout << endl << printData << endl;
			}
			else
			{
				cout << endl << checkData["error"].front() << endl;
			}
		}
	}
	else if (count == 3)
	{
		if (*ptr == "camera")
		{
			if (*++ptr == "fov")
			{
				++ptr;
				gSystem.camera.fov = atof((*ptr).c_str()) / 57.296575f;
				string printData = *++++++++checkData["camera"].begin();
				cout << endl << printData << gSystem.camera.fov * 57.296575f << endl;
			}
		}
	}
	else if (count == 4)
	{
		if (*ptr == "camera")
		{
			if (*++ptr == "coordinate")
			{
				auto tmp = ptr;
				if (!IsStr(*++++tmp))
				{
					float data = atof((*tmp).c_str());
					if (*++ptr == "x")
						gSystem.camera.position.x = data;
					else if (*ptr == "y")
						gSystem.camera.position.y = data;
					else if (*ptr == "z")
						gSystem.camera.position.z = data;
					else
					{
						cout << endl << checkData["error"].front() << endl;
						return;
					}
					string printData = checkData["camera"].front();
					cout << endl << printData << " x : " << gSystem.camera.position.x;
					cout << endl << printData << " y : " << gSystem.camera.position.y;
					cout << endl << printData << " z : " << gSystem.camera.position.z << endl;
				}
			}
			else if (*ptr == "speed")
			{
				auto tmp = ptr;
				if (!IsStr(*++++tmp))
				{
					float data = atof((*tmp).c_str());
					if (*++ptr == "xyz")
						gSystem.camera.speed.s = data;
					else if (*ptr == "theta")
						gSystem.camera.speed.th = data;
					else if (*ptr == "pi")
						gSystem.camera.speed.pi = data;
					else
					{
						cout << endl << checkData["error"].front() << endl;
						return;
					}
					string printData = *++checkData["camera"].begin();
					cout << endl << printData << " xyz : " << gSystem.camera.speed.s;
					cout << endl << printData << " theta : " << gSystem.camera.speed.th;
					cout << endl << printData << " pi : " << gSystem.camera.speed.pi << endl;
				}
				else
				{
					cout << endl << checkData["error"].front() << endl;
					return;
				}
			}
			else if (*ptr == "angle")
			{
				if (!IsStr(*++ptr))
				{
					float data_1 = atof((*ptr).c_str());
					if (!IsStr(*++ptr))
					{
						float data_2 = atof((*ptr).c_str());
						gSystem.camera.angle[0] = data_1;
						gSystem.camera.angle[0] = data_2;
					}
					else
					{
						cout << endl << checkData["error"].front() << endl;
						return;
					}
				}
				else
				{
					auto tmp = ptr;
					if (!IsStr(*++++tmp))
					{
						float data = atof((*tmp).c_str());
						if (*++ptr == "theta")
							gSystem.camera.angle[0] = data;
						else if (*ptr == "pi")
							gSystem.camera.angle[1] = data;
						else
						{
							cout << endl << checkData["error"].front() << endl;
							return;
						}
					}
				}
				string printData = *++++checkData["camera"].begin();
				cout << endl << printData << " theta : " << gSystem.camera.angle[0] * 57.296575f;
				cout << endl << printData << " pi : " << gSystem.camera.angle[1] * 57.296575f << endl;
			}
			else if (*ptr == "distance")
			{
				if (!IsStr(*++ptr))
				{
					float data_1 = atof((*ptr).c_str());
					if (!IsStr(*++ptr))
					{
						float data_2 = atof((*ptr).c_str());
						gSystem.camera.near_ = data_1;
						gSystem.camera.far_ = data_2;
					}
					else
					{
						cout << endl << checkData["error"].front() << endl;
						return;
					}
				}
				else
				{
					auto tmp = ptr;
					if (!IsStr(*++++tmp))
					{
						float data = atof((*tmp).c_str());
						if (*++ptr == "minimum")
							gSystem.camera.near_ = data;
						else if (*ptr == "maximum")
							gSystem.camera.far_ = data;
						else
						{
							cout << endl << checkData["error"].front() << endl;
							return;
						}
					}
					else
					{
						cout << endl << checkData["error"].front() << endl;
						return;
					}
				}
				string printData = *++++++checkData["camera"].begin();
				cout << endl << printData << " MAX : " << gSystem.camera.far_;
				cout << endl << printData << " MIN : " << gSystem.camera.near_ << endl;
			}
		}
	}
	else if (count == 5)
	{
		if (*ptr == "camera")
		{
			if (*++ptr == "coordinate")
			{
				auto tmp = ptr;
				if (!IsStr(*++tmp))
				{
					float data_1 = atof((*tmp).c_str());
					if (!IsStr(*++tmp))
					{
						float data_2 = atof((*tmp).c_str());
						if (!IsStr(*++tmp))
						{
							float data_3 = atof((*tmp).c_str());
							gSystem.camera.position.x = data_1;
							gSystem.camera.position.y = data_2;
							gSystem.camera.position.z = data_3;

							string printData = checkData["camera"].front();
							cout << endl << printData << " x : " << gSystem.camera.position.x;
							cout << endl << printData << " y : " << gSystem.camera.position.y;
							cout << endl << printData << " z : " << gSystem.camera.position.z << endl;
						}
						else
						{
							cout << endl << checkData["error"].front() << endl;
							return;
						}
					}
					else
					{
						cout << endl << checkData["error"].front() << endl;
						return;
					}
				}
				else
				{
					cout << endl << checkData["error"].front() << endl;
					return;
				}
			}
			else if (*ptr == "angle")
			{
				auto tmp = ptr;
				if (!IsStr(*++tmp))
				{
					float data_1 = atof((*tmp).c_str());
					if (!IsStr(*++tmp))
					{
						float data_2 = atof((*tmp).c_str());
						if (!IsStr(*++tmp))
						{
							float data_3 = atof((*tmp).c_str());
							gSystem.camera.speed.s = data_1;
							gSystem.camera.speed.th = data_2;
							gSystem.camera.speed.pi = data_3;

							string printData = *++checkData["camera"].begin();
							cout << endl << printData << " xyz : " << gSystem.camera.speed.s;
							cout << endl << printData << " theta : " << gSystem.camera.speed.th;
							cout << endl << printData << " pi : " << gSystem.camera.speed.pi << endl;
						}
						else
						{
							cout << endl << checkData["error"].front() << endl;
							return;
						}
					}
					else
					{
						cout << endl << checkData["error"].front() << endl;
						return;
					}
				}
				else
				{
					cout << endl << checkData["error"].front() << endl;
					return;
				}
			}
		}
	}
	gSystem.camera.SetPosition();
	cout << endl;
}

void CONSOLE::Initialize()
{
	SetFunction("CONSOLE::Initialize");

	if (gSystem.bConUsage == true)
	{
		AllocConsole();
		SetConsoleTitle("Debug Console");
		consoleHND = FindWindow(NULL, "Debug Console");
		MoveWindow(consoleHND, gSystem.winSize.right + gSystem.conSize.left, gSystem.conSize.top, gSystem.conSize.right, gSystem.conSize.bottom, TRUE);
		freopen("CONOUT$", "w", stdout);
		freopen("CONIN$", "r", stdin);
		freopen("CONERR$", "w", stderr);

		if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE)) 
		{
			gSystem.bConUsage = false;
		}
	}
	*this << con::info << con::func << "console started" << con::endl;

	//	Read console tab data txt file
	//
	std::ifstream file;
	string tmpPath;
	char input;
	string fileReadData;

	tmpPath = defaultPath;
	tmpPath += "CONSOLE_TAB_DATA.txt";
	file.open(tmpPath);

	if (file.fail())
	{
		file.clear();
		file.close();
		*this << con::error << con::func << "CONSOLE_TAB_DATA.txt open() - failed" << con::endl;
		*this << con::error << con::func << "console tab function disabled" << con::endl;
	}

	else
	{
		while (!file.eof())
		{
			file >> fileReadData;
			tabData.push_back(fileReadData);
		}
		file.clear();
		file.close();
		*this << con::info << con::func << "tab data load - succeed" << con::endl;
	}



	//	Read console check data txt file
	//
	file.open(defaultPath + "CONSOLE_CHECK_DATA.txt");
	if (file.fail())
	{
		file.clear();
		file.close();
		*this << con::error << con::func << "CONSOLE_CHECK_DATA open() - failed" << con::endl;
		*this << con::error << con::func << "console is now disabled" << con::endl;
		gSystem.bConUsage = false;
		DestroyWindow(consoleHND);
	}

	else
	{
		int num;
		list<string>		tmp_list;
		string				fileReadData_;
		map<string, list<string>> tmp_map;
		while (!file.eof())
		{
			file.get(input);
			if (input == ':')
			{
				file >> num;
				file >> fileReadData;
				file.get();
				for (int i = 0; i < num; i++)
				{
					fileReadData_.clear();
					while (true)
					{
						file.get(input);
						if (input == '\n' || file.eof())
							break;
						if (input == '\\')
						{
							file.get(input);
							if (input == '\n')
								break;
							if (input == 'n')
								fileReadData_ += '\n';
							else
							{
								fileReadData_ += '\\';
								fileReadData_ += input;
							}
						}
						else
							fileReadData_ += input;
					}
					tmp_list.push_back(fileReadData_);
				}
				checkData[fileReadData] = tmp_list;
				tmp_list.clear();
			}
		}
		file.clear();
		file.close();
		*this << con::info << con::func << "check data load - succeed" << con::endl;
		history.push_front(" ");
		*this << con::info << con::func << "console history ready" << con::endl;

		hisptr = history.begin();
	}
	RestoreFunction();
}
void CONSOLE::Release()
{
	if (gSystem.bConUsage == true)
		FreeConsole();
}

void CONSOLE::SetFunction(string _function)
{
	if (gSystem.bConUsage == true)
	{ 
		function_before = function;
		function = _function;
	}
}
void CONSOLE::RestoreFunction()
{
	if (gSystem.bConUsage == true)
	{
		function = function_before;
	}
}

void CONSOLE::PrintFunctionName()
{
	if (gSystem.bConUsage == false)
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
	if (gSystem.bConUsage == true)
		cout << _data;
	return *this;
}
CONSOLE& CONSOLE::operator<<(int _data)
{
	if (gSystem.bConUsage == true)
		cout << _data;
	return *this;
}
CONSOLE& CONSOLE::operator<<(float _data)
{
	if (gSystem.bConUsage == true)
		cout << _data;
	return *this;
}
CONSOLE& CONSOLE::operator<<(double _data)
{
	if (gSystem.bConUsage == true)
		cout << _data;
	return *this;
}
CONSOLE& CONSOLE::operator<<(con::ENDL_ _data)
{
	if (gSystem.bConUsage == true)
		cout << std::endl;
	return *this;
}
CONSOLE& CONSOLE::operator<<(con::FUNC_ _data)
{
	if (gSystem.bConUsage == true)
		PrintFunctionName();
	return *this;
}
CONSOLE& CONSOLE::operator<<(con::INFO_ _data)
{
	if (gSystem.bConUsage == true)
		cout << ":INFO";
	return *this;
}
CONSOLE& CONSOLE::operator<<(con::ERROR_ _data)
{
	if (gSystem.bConUsage == true)
		cout << ":ERROR";
	return *this;
}
CONSOLE& CONSOLE::operator<<(con::MSG_ _data)
{
	if (gSystem.bConUsage == true)
		cout << ":MSG ";
	return *this;
}
CONSOLE& CONSOLE::operator<<(POINT _data)
{
	if (gSystem.bConUsage == true)
		cout << "x : " << _data.x << " y : " << _data.y;
	return *this;
}
