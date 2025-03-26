#include "DungeonQueueSystem.h"
#include <iostream>
#include <limits>
#include <string>
#include <stdexcept>

unsigned int getPositiveInput(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt;

        // Read the entire line to handle various input scenarios
        if (!std::getline(std::cin, input)) {
            std::cout << "Error: Input reading failed.\n";
            continue;
        }

        // Remove leading and trailing whitespace
        input.erase(0, input.find_first_not_of(" \t"));
        input.erase(input.find_last_not_of(" \t") + 1);

        // Check if input is empty
        if (input.empty()) {
            std::cout << "Error: Input cannot be empty.\n";
            continue;
        }

        // Check if input contains only digits
        if (input.find_first_not_of("0123456789") != std::string::npos) {
            std::cout << "Error: Please enter a positive number.\n";
            continue;
        }

        // Check for overflow
        try {
            unsigned long long value = std::stoull(input);

            // Check if value exceeds unsigned int max
            if (value > std::numeric_limits<unsigned int>::max()) {
                std::cout << "Error: Number exceeds maximum allowed value ("
                    << std::numeric_limits<unsigned int>::max() << ").\n";
                continue;
            }

            // Check if value is positive
            if (value == 0) {
                std::cout << "Error: Please enter a positive number greater than 0.\n";
                continue;
            }

            return static_cast<unsigned int>(value);
        }
        catch (const std::out_of_range&) {
            std::cout << "Error: Number is too large.\n";
        }
        catch (const std::invalid_argument&) {
            std::cout << "Error: Invalid input.\n";
        }
    }
}

int main() {
    unsigned int n, t, h, d, t1, t2;

    do {
        n = getPositiveInput("Enter the maximum number of concurrent instances (n): ");
        t = getPositiveInput("Enter the number of tank players in queue (t): ");
        h = getPositiveInput("Enter the number of healer players in queue (h): ");
        d = getPositiveInput("Enter the number of DPS players in queue (d): ");
        t1 = getPositiveInput("Enter the minimum time before an instance is finished (t1): ");
        t2 = getPositiveInput("Enter the maximum time before an instance is finished (t2): ");

        if (t2 > 15) {
            std::cout << "Warning: t2 exceeds the recommended maximum of 15 seconds\n";
        }

        if (t >= 1 && h >= 1 && d >= 3) {
            break; // Valid configuration, proceed to start the system
        }
        else {
            std::cout << "Error: Not enough players to start a dungeon party.\n";
            std::cout << "  Required: 1 tank, 1 healer, 3 DPS\n";
            std::cout << "  Current queue: "
                << "Tanks: " << t
                << ", Healers: " << h
                << ", DPS: " << d << "\n";
            std::cout << "Please re-enter the values.\n\n";
        }
    } while (true);

    DungeonQueueSystem system(n, t, h, d, t1, t2);
    system.start();

    return 0;
}