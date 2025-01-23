#pragma once
#include <string>

class IHttpRequest
{
public:
    virtual ~IHttpRequest() = default;

    virtual std::string PerformRequest(const std::string &url) = 0;
};