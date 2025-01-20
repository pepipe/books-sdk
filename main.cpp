#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

size_t writeCallback(void* contents, const size_t size, const size_t nmemb, std::string* userp) {
    userp->append(static_cast<char *>(contents), size * nmemb);
    return size * nmemb;
}

int main() {
    CURL* curl = curl_easy_init();
    std::string response;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://jsonplaceholder.typicode.com/posts/1");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            json data = json::parse(response);
            std::cout << "Title: " << data["title"] << std::endl;
        } else {
            std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
    }
    return 0;
}