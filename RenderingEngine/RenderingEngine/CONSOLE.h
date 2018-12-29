#ifndef __CONSOLE_H__
#define __CONSOLE_H__
#define PRINTSIZE 18

#include<string>
#include<Windows.h>
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
		string function;
		string function_before;

		void setFunction(string);
		void restoreFunction();
		void print();

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