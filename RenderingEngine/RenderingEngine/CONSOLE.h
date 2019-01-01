#ifndef __CONSOLE_H__
#define __CONSOLE_H__
#define PRINTSIZE 20
#define MAXFILEREAD 256
#include<string>
#include<Windows.h>
#include<list>
#include<map>
using namespace std;

namespace con
{
	struct FUNC_ { FUNC_() {} };
	struct ENDL_ { ENDL_() {} };
	struct INFO_ { INFO_() {} };
	struct ERROR_ { ERROR_() {} };
	struct MSG_ { MSG_() {} };

	extern ENDL_ endl;
	extern FUNC_ func;
	extern INFO_ info;
	extern ERROR_ error;
	extern MSG_ msg;
}
class CONSOLE
{
public:
	HWND			consoleHND;

	string defaultPath = "Data\\Console\\";
	map<string, list<string>> checkData;
	string			function;
	string			function_before;

	list<string>	history;
	list<string>::iterator hisptr;

	string			userinput;
	list<string>	outinput;
	list<string>	tabData;

	void Input();
	void Check();


	void Initialize();
	void Release();
	void SetFunction(string);
	void RestoreFunction();
	void Print();
	bool IsStr(string);



	CONSOLE& operator<<(int);
	CONSOLE& operator<<(string);
	CONSOLE& operator<<(float);
	CONSOLE& operator<<(double);
	CONSOLE& operator<<(con::ENDL_);
	CONSOLE& operator<<(con::FUNC_);
	CONSOLE& operator<<(con::INFO_);
	CONSOLE& operator<<(con::ERROR_);
	CONSOLE& operator<<(con::MSG_);

	CONSOLE& operator>>(int);
	CONSOLE& operator>>(string);
	CONSOLE& operator>>(float);
	CONSOLE& operator>>(double);
	CONSOLE& operator>>(con::ENDL_);
	CONSOLE& operator>>(con::FUNC_);
	CONSOLE& operator>>(con::INFO_);
	CONSOLE& operator>>(con::ERROR_);
	CONSOLE& operator>>(con::MSG_);
	CONSOLE& operator>>(POINT);


	CONSOLE();
	~CONSOLE();
};


#endif // !__CONSOLE_H__