#pragma once

#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;

/**
 * @class Config
 * @brief Manages application configuration loaded from a YAML file.
 *
 * This class is responsible for locating, loading, and accessing configuration
 * parameters stored in a YAML file. It uses C++17 std::filesystem for file
 * path handling and yaml-cpp library for parsing YAML content.
 *
 * Typical usage:
 * - Instantiate Config with an application name and config file name.
 * - Access configuration values by key using get().
 *
 */
class Config {
   private:
    fs::path configFilePath;  // Full path to the configuration YAML file.
    fs::path
        configDirpath;  // Directory path containing the configuration file.
    const char*
        HOME_DIR;  // Pointer to the user's home directory environment variable.

    YAML::Node config;  // Parsed YAML node containing the config data.

   public:
    /**
     * @brief Constructs a Config object by loading a YAML config file.
     *
     * @param appName The name of the application (used to resolve config path).
     * @param configFile The filename of the configuration YAML file.
     *
     * Loads and parses the YAML configuration from the specified file.
     */
    Config(std::string appName, std::string configFile);

    /**
     * @brief Destructor for Config.
     *
     * Performs any necessary cleanup.
     */
    ~Config();

    /**
     * @brief Retrieves the value associated with a given key from the config.
     *
     * @param key The configuration key to retrieve.
     * @return std::string The corresponding value as a string.
     *
     * Throws an exception or returns an empty string if the key is not found,
     * depending on implementation.
     */
    std::string get(std::string key);
};
