#ifndef __linux__
#error "This app only supports Linux."
#endif

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#include "config.hpp"
#include "notify.hpp"

/**
 * @brief Prints the help message explaining usage and options.
 *
 * @param prog_name The name of the executable (argv[0])
 */
void print_help(const char* prog_name) {
    std::cout
        << "Usage:\n"
        << "  " << prog_name << " <message> [-p priority] [-t title]\n\n"
        << "Options:\n"
        << "  <message>            The notification message (required, first "
           "argument)\n"
        << "  -p, --priority NUM   Priority level (1-5). Default: 3 (DEFAULT)\n"
        << "  -t, --title STRING   Notification title (optional)\n"
        << "  -h, --help           Show this help message\n";
}

/**
 * @brief Entry point of the CLI application.
 *
 * Parses command line arguments to extract:
 * - Message (positional, required)
 * - Priority (-p or --priority, optional, default 3)
 * - Title (-t or --title, optional)
 * - Help (-h or --help)
 *
 * Validates arguments and shows help on errors.
 *
 * @param argc Argument count
 * @param argv Argument vector (array of C-style strings)
 * @return int Exit status code
 */
int main(int argc, char* argv[]) {
    // Require at least one argument (the message)
    if (argc < 2) {
        print_help(argv[0]);
        return 1;
    }

    // If first argument starts with '-', treat it as an option
    // Handle help or error for missing required message
    if (argv[1][0] == '-') {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            print_help(argv[0]);
            return 0;
        } else {
            std::cerr << "Error: Missing required message argument.\n\n";
            print_help(argv[0]);
            return 1;
        }
    }

    // Initialize variables for CLI options
    std::string message = argv[1];  // First positional argument = message
    std::string title;              // Optional notification title
    Priority priority = Priority::DEFAULT;  // Default priority (3)

    // Parse remaining arguments as options
    for (int i = 2; i < argc; ++i) {
        // Priority option: -p or --priority
        if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--priority") == 0) {
            if (i + 1 >= argc) {  // Missing value for priority
                std::cerr << "Error: Missing value for " << argv[i] << "\n\n";
                print_help(argv[0]);
                return 1;
            }
            int p = std::atoi(argv[i + 1]);  // Convert string to int
            if (p < 1 || p > 5) {            // Validate priority range
                std::cerr << "Error: Priority must be between 1 and 5\n\n";
                print_help(argv[0]);
                return 1;
            }
            priority = static_cast<Priority>(p);
            i++;  // Skip the value argument
        }
        // Title option: -t or --title
        else if (strcmp(argv[i], "-t") == 0 ||
                 strcmp(argv[i], "--title") == 0) {
            if (i + 1 >= argc) {  // Missing value for title
                std::cerr << "Error: Missing value for " << argv[i] << "\n\n";
                print_help(argv[0]);
                return 1;
            }
            title = argv[i + 1];
            i++;  // Skip the value argument
        }
        // Help option: -h or --help
        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_help(argv[0]);
            return 0;
        }
        // Unknown option
        else {
            std::cerr << "Error: Unknown option '" << argv[i] << "'\n\n";
            print_help(argv[0]);
            return 1;
        }
    }

    // Load configuration (from config.yaml in "notify" config directory)
    Config config("notify", "config.yaml");

    // Call notify function with parsed parameters
    int ret = notify(config, message, title, priority);
    return ret;  // Return notify's status code
}
