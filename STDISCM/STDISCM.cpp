#include "DungeonQueueSystem.h"
#include <iostream>

int main() {
    unsigned int n, t, h, d, t1, t2;

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

    DungeonQueueSystem system(n, t, h, d, t1, t2);
    system.start();

    return 0;
}
