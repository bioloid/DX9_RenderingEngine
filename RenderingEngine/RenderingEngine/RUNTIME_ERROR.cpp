#include "RUNTIME_ERROR.h"
string RUNTIME_ERROR::info()
{
	switch (error)
	{
	case CRITICAL_CLASS_ERROR:
		return "critical class error";
	case CRITICAL_WINDOW_ERROR:
		return "critical window error";
	default:
		break;
	}
}