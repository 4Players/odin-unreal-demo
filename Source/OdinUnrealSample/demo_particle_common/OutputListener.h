#pragma once

#include <iostream>
#include "Common-cpp/inc/Common.h"
#if defined _EG_ANDROID_PLATFORM
#	include <android/log.h>
#elif defined _EG_SWITCH_WINDOWS_PLATFORM
#	include <nn/nn_Log.h>
#endif

class OutputListener : public virtual ExitGames::Common::BaseListener
{
protected:
	void log(const ExitGames::Common::JString& s, FColor color)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, color, s.UTF8Representation().cstr());
	}
public:
	virtual ~OutputListener(void){};
	virtual void write(const ExitGames::Common::JString& str) = 0;
	virtual void writeLine(const ExitGames::Common::JString& str=ExitGames::Common::JString()) = 0;
	virtual void debugReturn(int debugLevel, const ExitGames::Common::JString& str)
	{
		switch(debugLevel)
		{
		case ExitGames::Common::DebugLevel::ALL:
			log(ExitGames::Common::JString(L"DEBUG: ")+str, FColor::White);
			break;
		case ExitGames::Common::DebugLevel::INFO:
			log(ExitGames::Common::JString(L"INFO: ")+str, FColor::Green);
			break;
		case ExitGames::Common::DebugLevel::WARNINGS:
			log(ExitGames::Common::JString(L"WARN: ")+str, FColor::Yellow);
			break;
		case ExitGames::Common::DebugLevel::ERRORS:
			log(ExitGames::Common::JString(L"ERROR: ")+str, FColor::Red);
			break;
		case ExitGames::Common::DebugLevel::OFF:
		default:
			log(ExitGames::Common::JString(L"FATAL: ")+str, FColor::Black);
			break;
		}
#	if defined _EG_SWITCH_WINDOWS_PLATFORM
		NN_LOG((str+L"\n").UTF8Representation().cstr());
#	else
		std::wcerr << str << std::endl;
#	endif
#	if defined _EG_WINDOWS_PLATFORM || defined _EG_WINDOWSSTORE_PLATFORM || defined _EG_XB1_PLATFORM
		OutputDebugStringW((str+L"\n").cstr());
#	endif
#	if defined _EG_ANDROID_PLATFORM
		int logLevel;
		switch(debugLevel)
		{
		case ExitGames::Common::DebugLevel::ALL:
			logLevel = ANDROID_LOG_DEBUG;
			break;
		case ExitGames::Common::DebugLevel::INFO:
			logLevel = ANDROID_LOG_INFO;
			break;
		case ExitGames::Common::DebugLevel::WARNINGS:
			logLevel = ANDROID_LOG_WARN;
			break;
		case ExitGames::Common::DebugLevel::ERRORS:
			logLevel = ANDROID_LOG_ERROR;
			break;
		case ExitGames::Common::DebugLevel::OFF:
			logLevel = ANDROID_LOG_FATAL;
			break;
		default:
			logLevel = ANDROID_LOG_FATAL;
			__android_log_write(logLevel, "Photon-Demo", "unknown log level");
			break;
		}
		__android_log_write(logLevel, "Photon-Demo", str.UTF8Representation().cstr());
#	endif
	}
};