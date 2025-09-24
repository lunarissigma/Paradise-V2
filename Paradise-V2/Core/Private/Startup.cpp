#include "../Public/Startup.h"
#include "../Public/Redirect.h"
#include "../Globals.h"
#include "../Public/Finder.h"

void Startup::Init() {
    const std::vector<uint8_t> PATTERN_1 = { 0x48, 0x81, 0xEC };
    const std::vector<uint8_t> PATTERN_2 = { 0x40 };
    const std::vector<uint8_t> PATTERN_3 = { 0x48, 0x8B, 0xC4 };
    const std::vector<uint8_t> PATTERN_4 = { 0x4C, 0x8B, 0xDC };
    const std::vector<uint8_t> PATTERN_5 = { 0x48, 0x83, 0xEC };

    auto scanner = Memcury::Scanner::FindStringRef(Offsets::ProcessRequestStat, Memcury::PE::GetModuleBase(), false);

    if (!scanner.IsValid()) {
        scanner = Memcury::Scanner::FindStringRef(Offsets::ProcessRequest);
        Redirect::ProcessRequestOG = scanner
            .ScanFor(PATTERN_1, false)
            .ScanFor(PATTERN_2, false)
            .GetAs<decltype(Redirect::ProcessRequestOG)>();
    }
    else {
        Redirect::ProcessRequestOG = scanner
            .ScanFor(PATTERN_1, false)
            .ScanFor(PATTERN_2, false)
            .GetAs<decltype(Redirect::ProcessRequestOG)>();
    }

    if (auto targetPtr = Redirect::ProcessRequestOG) {
        auto ref = Memcury::Scanner::FindPointerRef(targetPtr).GetAs<void**>();
        if (!ref) {
            if (scanner.IsValid()) {
                Redirect::ProcessRequestOG = scanner
                    .ScanFor(PATTERN_5, false)
                    .ScanFor(PATTERN_2, false)
                    .GetAs<decltype(Redirect::ProcessRequestOG)>();
                ref = Memcury::Scanner::FindPointerRef(targetPtr).GetAs<void**>();
                if (!ref) {
                    Redirect::ProcessRequestOG = scanner
                        .ScanFor(PATTERN_3, false)
                        .GetAs<decltype(Redirect::ProcessRequestOG)>();
                    ref = Memcury::Scanner::FindPointerRef(targetPtr).GetAs<void**>();
                    if (!ref) {
                        Redirect::ProcessRequestOG = scanner
                            .ScanFor(PATTERN_4, false)
                            .GetAs<decltype(Redirect::ProcessRequestOG)>();
                    }
                }
            }
        }

        if (ref) {
            DWORD oldProtect;
            if (VirtualProtect(ref, sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtect)) {
                *ref = reinterpret_cast<void*>(Redirect::ProcessRequest);
                VirtualProtect(ref, sizeof(void*), oldProtect, &oldProtect);
            }
        }
    }
    else {
        *Memcury::Scanner::FindPointerRef(targetPtr).GetAs<void**>() = Redirect::ProcessRequest;
        Finder::InitializeExitHook();
    }
}