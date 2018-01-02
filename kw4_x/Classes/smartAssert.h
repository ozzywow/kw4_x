#pragma once
#include "cocos2d.h"
#include <string>
#include <stdarg.h>
using namespace cocos2d;

std::string format_arg_listA(const char *szFormat, va_list args)
{
	const size_t tBuffSize = 1024;
	char szBuff[tBuffSize];
    
	vsprintf(szBuff, szFormat, args);
	std::string s(szBuff);
	return s;
}

std::string formatA(const char *szFormat, ...)
{
	va_list pVAList;
	va_start(pVAList, szFormat);
	std::string s = format_arg_listA(szFormat, pVAList);
	va_end(pVAList);
	return s;
}

#define USE_SMART_ASSERTION

#ifndef USE_SMART_ASSERTION

#define assertSmartly(condition)
#define assertSmartlyWithMsg(condition, message)

#else

#define assertSmartly(condition) \
{ \
	if (!(condition)) \
	{ \
		std::string msg = formatA("File : %s\r\nLine : %d", __FILE__, __LINE__); \
		MessageBox(msg.c_str(), "assertion failed"); \
		Director::getInstance()->stopAnimation(); \
		Director::getInstance()->pause(); \
	} \
}

#define assertSmartlyWithMsg(condition, message) \
{ \
	if (!(condition)) \
	{ \
		const char * userMsg = (const char *)message; \
		std::string msg = formatA("File : %s\r\nLine : %d\r\nMessage : %s", __FILE__, __LINE__, userMsg); \
		MessageBox(msg.c_str(), "assertion failed"); \
		Director::getInstance()->stopAnimation(); \
		Director::getInstance()->pause(); \
	} \
}

#endif

