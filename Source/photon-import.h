#pragma once

#ifdef __clang__
#pragma clang diagnostic ignored "-Woverloaded-virtual"
#endif

#if _EG_WINDOWS_PLATFORM
#include "AllowWindowsPlatformTypes.h"
#pragma warning (disable: 4263)
#pragma warning (disable: 4264)
#endif
#include "LoadBalancing-cpp/inc/Client.h"
#if _EG_WINDOWS_PLATFORM
#pragma warning (default: 4263)
#pragma warning (default: 4264)
#include "HideWindowsPlatformTypes.h"
#endif