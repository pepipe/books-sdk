#include "CurlHttpRequest.h"

#include <stdexcept>
#include <curl/curl.h>

#include "android/AndroidLogging.h"

// Helper function to write cURL response to a string
size_t WriteCallback(void *contents, const size_t size, const size_t nmemb, std::string *s)
{
    const size_t totalSize = size * nmemb;
    s->append(static_cast<char *>(contents), totalSize);
    return totalSize;
}

std::string CurlHttpRequest::PerformRequest(const std::string &url)
{
    std::string response;

    if (CURL *curl = curl_easy_init())
    {
        long http_code = 0;
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // 🔴 Disable SSL verification (temporary fix)
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        // Perform the request
        const CURLcode res = curl_easy_perform(curl);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        curl_easy_cleanup(curl);

#ifdef ANDROID
        LOGI("cURL Response Code: %ld", http_code);
#endif
        if (res != CURLE_OK)
        {
#ifdef ANDROID
            LOGI("cURL Error: %s", curl_easy_strerror(res));
#endif
            throw std::runtime_error("Error performing request: " + std::string(curl_easy_strerror(res)));
        }
    }

    return response;
}
