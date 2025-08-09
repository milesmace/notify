#pragma once

#include "config.hpp"

enum class Priority { MIN = 1, LOW, DEFAULT, HIGH, MAX };

int notify(Config& config,
           const std::string& message,
           const std::string& title = "",
           Priority priority = Priority::DEFAULT);
