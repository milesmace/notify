#include "notify.hpp"

#include <curl/curl.h>

#include <iostream>
#include <string>

#include "config.hpp"

size_t discardResponseCallback(void* contents, size_t size, size_t nmemb,
                               void* userp) {
    return size * nmemb;
}

int notify(Config& config, const std::string& message, const std::string& title,
           Priority priority) {
    std::string serverUrl =
        config.get("default_server") + config.get("default_channel");
    if (serverUrl.empty()) {
        std::cerr << "Error: server URL or channel missing in config"
                  << std::endl;
        return 1;
    }

    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to init curl" << std::endl;
        return 1;
    }

    // Compose headers: set priority in header 'Priority: ...'
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(
        headers, ("Priority: " + std::to_string((int) priority)).c_str());

    // If title is provided, use it as the message title (can be in the payload
    // or header, ntfy supports title as header "Title")
    if (!title.empty()) {
        headers = curl_slist_append(headers, ("Title: " + title).c_str());
    }

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, discardResponseCallback);
    curl_easy_setopt(curl, CURLOPT_URL, serverUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, message.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long) message.size());

    // Optional: timeout, verbose for debugging
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
    // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res)
                  << std::endl;
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        return 1;
    }

    // Cleanup
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    std::cout << "Notification sent successfully" << std::endl;
    return 0;
}
