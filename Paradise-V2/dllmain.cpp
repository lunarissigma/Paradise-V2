#include "framework.h"
#include "Core/Public/Startup.h"
#include "Core/Public/Finder.h"
#include <thread>
#include "Core/Globals.h"

void Main()
{
    AllocConsole();
    SetConsoleTitleA("Paradise-V2 | github.com/lunarissigma/Paradise-V2");
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    Startup::Init();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD Reason, LPVOID Reserved)
{
    switch (Reason)
    {
    case DLL_PROCESS_ATTACH:
        std::thread(Main).detach();
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
