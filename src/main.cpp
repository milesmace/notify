#ifndef __linux__
#error "This app only supports Linux."
#endif

#include <iostream>

#include "config.hpp"

int main() {
    Config config("notify", "config.yaml");
    config.get("random_shit");
    std::string serverUrl =
        config.get("default_server") + config.get("default_channel");

    std::cout << serverUrl << std::endl;

    return 0;
}
