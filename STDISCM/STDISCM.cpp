#include "DungeonQueueSystem.h"
#include <iostream>

int main() {
    unsigned int n, t, h, d, t1, t2;

    do {
        std::cout << "Enter the maximum number of concurrent instances (n): ";
        std::cin >> n;

        std::cout << "Enter the number of tank players in queue (t): ";
        std::cin >> t;

        std::cout << "Enter the number of healer players in queue (h): ";
        std::cin >> h;

        std::cout << "Enter the number of DPS players in queue (d): ";
        std::cin >> d;

        std::cout << "Enter the minimum time before an instance is finished (t1): ";
        std::cin >> t1;

        std::cout << "Enter the maximum time before an instance is finished (t2): ";
        std::cin >> t2;

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

    } while (true); // Repeat input until valid player counts are given

    DungeonQueueSystem system(n, t, h, d, t1, t2);
    system.start();

    return 0;
}
