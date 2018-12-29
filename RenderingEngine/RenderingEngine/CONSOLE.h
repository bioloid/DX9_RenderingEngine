#ifndef __CONSOLE_H__
#define __CONSOLE_H__
#define PRINTSIZE 12

#include<string>
using namespace std;

namespace con
{
	struct FUNC_ { FUNC_() {} };
	struct ENDL_ { ENDL_() {} };
	struct INFO_ { INFO_() {} };
	struct ERROR_ { ERROR_() {} };

	extern ENDL_ endl;
	extern FUNC_ func;
	extern INFO_ info;
	extern ERROR_ error;

	class CONSOLE
	{
	public:
		string function;


		void setFunction(string);
		void print();

		CONSOLE& operator<<(int);
		CONSOLE& operator<<(string);
		CONSOLE& operator<<(float);
		CONSOLE& operator<<(double);
		CONSOLE& operator<<(ENDL_);
		CONSOLE& operator<<(FUNC_);
		CONSOLE& operator<<(INFO_);
		CONSOLE& operator<<(ERROR_);

		CONSOLE& operator>>(int);
		CONSOLE& operator>>(string);
		CONSOLE& operator>>(float);
		CONSOLE& operator>>(double);


		CONSOLE();
		~CONSOLE();
	};

}

#endif // !__CONSOLE_H__