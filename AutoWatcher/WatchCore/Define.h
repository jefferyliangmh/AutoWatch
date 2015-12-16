#pragma once
#ifdef _WIN32
	#ifdef WATHCORE_EXPORTS
		#define WATCHAPI __declspec(dllexport)
	#else
		#define WATCHAPI __declspec(dllimport)
	#endif // WATHCORE_EXPORTS
#else
	#define WATCHAPI
#endif // _WIN32

#include "../Logger/Log.h"