#ifndef __linux__
#error "This app only supports Linux."
#endif

#include <iostream>

#include "config.hpp"
#include "notify.hpp"

int main() {
    Config config("notify", "config.yaml");
    std::string serverUrl =
        config.get("default_server") + config.get("default_channel");

    notify(config, "Hello world", "My Title", Priority::MIN);

    return 0;
}
