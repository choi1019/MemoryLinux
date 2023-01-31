#pragma once

#include <01Base/Aspect/Log.h>

#if _TESTDEBUG
	#define TESTLOG_HEADER(...) Log().PrintSeparator(); Log(__VA_ARGS__).PrintHeader(); Log().PrintSeparator();
	#define TESTLOG_FOOTER(...) Log().PrintSeparator(); Log(__VA_ARGS__).PrintFooter(); Log().PrintSeparator();
	#define TESTLOG_NEWLINE(...) Log(__VA_ARGS__).Println()
	#define TESTLOG(...) Log(__VA_ARGS__).Print()
//	#define TESTLOG_TIME(...) Log(__VA_ARGS__).PrintTime()
#else
	#define TESTLOG_HEADER(CLASSNAME, ...)
	#define TESTLOG_FOOTER(...)
	#define TESTLOG_NEWLINE(CLASSNAME, ...)
	#define TESTLOG(CLASSNAME, ...)
//	#define TESTLOG_TIME(...) Log(__VA_ARGS__).PrintTime()
#endif

