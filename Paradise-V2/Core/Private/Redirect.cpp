#include "../Public/Redirect.h"
#include "../Public/Finder.h"
#include "../Globals.h"

void InternalProcessRequest(Containers::FCurlHttpRequest* Request, bool bEOS)
{
    static int RequestCount = 0;
    RequestCount++;

    if (RequestCount == 10) {
        auto EOSHandle = reinterpret_cast<uintptr_t>(GetModuleHandleA("EOSSDK-Win64-Shipping.dll"));
        if (EOSHandle) {
            auto scanner = Memcury::Scanner::FindStringRef(
                L"ProcessRequest failed. URL '%s' is not using a whitelisted domain. %p",
                EOSHandle
            );

            Redirect::EOSProcessRequestOG = scanner
                .ScanFor({ 0x48, 0x89, 0x5c }, false)
                .GetAs<decltype(Redirect::EOSProcessRequestOG)>();

            auto ref = Memcury::Scanner::FindPointerRef(Redirect::EOSProcessRequestOG, EOSHandle).GetAs<void**>();
            if (ref) {
                DWORD oldProtect;
                if (VirtualProtect(ref, sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtect)) {
                    *ref = reinterpret_cast<void*>(Redirect::EOSProcessRequest);
                    VirtualProtect(ref, sizeof(void*), oldProtect, &oldProtect);
                    Log("Successfully hooked EOS ProcessRequest");
                }
            }
        }
    }

    auto check = [](const std::wstring& fullUrl) -> std::pair<bool, size_t> {
        static const std::wstring domains[] = {
            L"game-social.epicgames.com",
            L"ol.epicgames.com",
            L"ol.epicgames.net",
            L"on.epicgames.com",
            L"ak.epicgames.com",
            L"epicgames.dev"
        };

        for (const auto& d : domains) {
            size_t pos = fullUrl.find(d);
            if (pos != std::wstring::npos) {
                return { true, pos + d.length() };
            }
        }
        return { false, 0 };
    };

    std::wstring URL = Request->GetURL().ToString();
    static bool bInitExit = false;

    auto result = check(URL);
    if (result.first) {
        size_t end = result.second;
        std::wstring newPath = URL.substr(end);

        if (newPath.find(L"dedicated_server") != std::wstring::npos) {
            if (!bInitExit) {
                bInitExit = true;
                Finder::InitializeExitHook();
            }
        }

        std::wstring newUrl = Options::RedirectURL + newPath;
        Request->SetURL(newUrl.c_str(), bEOS);
    }
}

bool Redirect::ProcessRequest(Containers::FCurlHttpRequest* Request)
{
    InternalProcessRequest(Request, false);
    return ProcessRequestOG(Request);
}

bool Redirect::EOSProcessRequest(Containers::FCurlHttpRequest* Request)
{
    InternalProcessRequest(Request, true);
    return EOSProcessRequestOG(Request);
}
