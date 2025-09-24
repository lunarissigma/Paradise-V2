#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <thread>
#include <string>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <map>
#include <intrin.h>
#include <sstream>
#include <utility> 
#include <array>
#include <tlhelp32.h>
#include <future>
#include <iomanip>  
#include <set>
#include "detours.h"
#include "MinHook.h"
#include "Utilities/memcury.h"

#pragma comment(lib, "detours.lib")
#pragma comment(lib, "minhook.lib")

#define Log(...) do { \
    std::ostringstream ss; \
    ss << __VA_ARGS__; \
    printf("[Paradise] %s (%s:%d) --> %s\n", __func__, __FILE__, __LINE__, ss.str().c_str()); \
} while(0)