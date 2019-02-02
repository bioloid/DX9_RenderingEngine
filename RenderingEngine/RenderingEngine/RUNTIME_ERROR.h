#ifndef __RUNTIME_ERROR_H__
#define __RUNTIME_ERROR_H__


#include "GAMESYSTEM.h"

enum errorCode {
	//  UNFIXABLE ERROR
	CRITICAL_CLASS_ERROR = -10,
	CRITICAL_WINDOW_ERROR,
	CRITICAL_DIRECTX_ERROR,

	//	GAME ERROR
	MODEL_LOAD_ERROR = 1,
	

	//  WINDOW ERROR
	REGISTER_WINDOW_CLASS_ERROR = 20,
	CREATE_WINDOW_ERROR,

	//  CLASS ERROR
	CONSOLE_TAB_DATA_OPEN_ERROR = 40,
	CONSOLE_CHECK_DATA_OPEN_ERROR,
	CONSOLE_HANDLE_SET_ERROR,
};

class RUNTIME_ERROR
{
public:
	errorCode error;
	RUNTIME_ERROR(errorCode _error) { error = _error; }
	~RUNTIME_ERROR() {}
	string info();
};

#endif