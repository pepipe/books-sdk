#pragma once
#include "IHttpRequest.h"

class CurlHttpRequest final : public IHttpRequest
{
public:
    std::string PerformRequest(const std::string &url) override;
};