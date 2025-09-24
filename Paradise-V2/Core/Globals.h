#pragma once
#include "Public/Containers.h"

namespace Offsets {
    inline void RequestExitWithStatus(bool, unsigned char) {}
    inline void UnsafeEnvironmentPopup(wchar_t**, unsigned __int8*, __int64*, char) {};

    constexpr auto ProcessRequest = L"Could not set libcurl options for easy handle, processing HTTP request failed. Increase verbosity for additional information.";
    constexpr auto ProcessRequestStat = L"STAT_FCurlHttpRequest_ProcessRequest";
}

namespace Options {
    inline const wchar_t* RedirectURL = L"http://127.0.0.1:3551";
    static bool bConsole = true;
}