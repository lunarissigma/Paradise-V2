#pragma once
#include "Containers.h"

namespace Redirect
{
    bool ProcessRequest(Containers::FCurlHttpRequest* Request);
    bool EOSProcessRequest(Containers::FCurlHttpRequest* Request);

    inline bool (*ProcessRequestOG)(Containers::FCurlHttpRequest* Request) = nullptr;
    inline bool (*EOSProcessRequestOG)(Containers::FCurlHttpRequest* Request) = nullptr;
}
