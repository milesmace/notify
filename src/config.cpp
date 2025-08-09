#include "config.hpp"

Config::Config(std::string appName, std::string configFile = "config.yaml") {
    // Set HOME environment
    HOME_DIR = getenv("HOME");
    if (!HOME_DIR) {
        throw std::runtime_error("HOME environment variable not set");
        exit(1);
    }

    this->configDirpath = fs::path(HOME_DIR) / ".config" / appName;
    this->configFilePath = this->configDirpath / configFile;

    // Check if the file exists or not
    if (!fs::exists(this->configFilePath)) {
        // Create a new one
        fs::create_directory(this->configDirpath);

        // Write default config
        std::ofstream configFile(this->configFilePath);

        configFile << "# Default settings for `" << appName << "`\n\n";
        configFile << "default_server: https://ntfy.sh/\n";
        configFile << "default_channel: test\n\n";

        // Close the file
        configFile.close();
    }

    // Load the config file
    this->config = YAML::LoadFile(this->configFilePath.string());
}

Config::~Config() {
    // TODO: Add cleanup
}

std::string Config::get(std::string key) {
    return this->config[key].as<std::string>();
}
