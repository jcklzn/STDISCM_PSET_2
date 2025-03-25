#include "DungeonQueueSystem.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <algorithm>

DungeonQueueSystem::Instance::Instance()
    : active(false), partiesServed(0), totalTimeServed(0),
    tankId(0), healerId(0), dpsIds(3, 0) {}

void DungeonQueueSystem::Instance::reset() {
    *this = Instance();
}

DungeonQueueSystem::DungeonQueueSystem(unsigned int maxInstances, unsigned int tanks,
    unsigned int healers, unsigned int dps,
    unsigned int minTime, unsigned int maxTime)
    : n(maxInstances), t1(minTime), t2(maxTime),
    running(true), rng(std::random_device()()) {

    instances.resize(n);
    timeDist = std::uniform_int_distribution<unsigned int>(t1, t2);

    for (unsigned int i = 1; i <= tanks; ++i)
        tankQueue.push(i);
    for (unsigned int i = 1; i <= healers; ++i)
        healerQueue.push(i + tanks);
    for (unsigned int i = 1; i <= dps; ++i)
        dpsQueue.push(i + tanks + healers);
}

unsigned int DungeonQueueSystem::findAvailableInstance() {
    auto it = std::find_if(instances.begin(), instances.end(),
        [](const Instance& inst) { return !inst.active; });

    return (it != instances.end()) ? std::distance(instances.begin(), it) : -1;
}

bool DungeonQueueSystem::canFormParty() const {
    return tankQueue.size() && healerQueue.size() && dpsQueue.size() >= 3;
}

void DungeonQueueSystem::formParty(unsigned int instanceId) {
    auto& instance = instances[instanceId];

    instance.tankId = tankQueue.front(); tankQueue.pop();
    instance.healerId = healerQueue.front(); healerQueue.pop();

    for (int i = 0; i < 3; ++i) {
        instance.dpsIds[i] = dpsQueue.front();
        dpsQueue.pop();
    }

    instance.active = true;
    unsigned int clearTime = timeDist(rng);
    instance.totalTimeServed += clearTime;
    instance.partiesServed++;

    // Existing print statements remain unchanged
    std::cout << "Party formed and assigned to instance " << instanceId + 1 << ":\n";
    std::cout << "  Tank: Player " << instance.tankId << "\n";
    std::cout << "  Healer: Player " << instance.healerId << "\n";
    std::cout << "  DPS: Players ";
    for (unsigned int i = 0; i < 3; i++) {
        std::cout << instance.dpsIds[i];
        if (i < 2) std::cout << ", ";
    }
    std::cout << "\n";
    std::cout << "  Clear time: " << clearTime << " seconds\n\n";

    std::thread([this, instanceId, clearTime]() {
        std::this_thread::sleep_for(std::chrono::seconds(clearTime));

        std::unique_lock<std::mutex> lock(mtx);
        instances[instanceId].reset();
        lock.unlock();
        cv.notify_all();
        }).detach();
}

void DungeonQueueSystem::start() {
    std::cout << "Starting dungeon queue system with:\n";
    std::cout << "  Maximum instances: " << n << "\n";
    std::cout << "  Tanks in queue: " << tankQueue.size() << "\n";
    std::cout << "  Healers in queue: " << healerQueue.size() << "\n";
    std::cout << "  DPS in queue: " << dpsQueue.size() << "\n";
    std::cout << "  Clear time range: " << t1 << "-" << t2 << " seconds\n\n";

    // Check if there are enough players to start
    if (!tankQueue.size() || !healerQueue.size() || dpsQueue.size() < 3) {
        std::cout << "Error: Not enough players to start a dungeon party.\n";
        std::cout << "  Required: 1 tank, 1 healer, 3 DPS\n";
        std::cout << "  Current queue: "
            << "Tanks: " << tankQueue.size()
            << ", Healers: " << healerQueue.size()
            << ", DPS: " << dpsQueue.size() << "\n";
        running = false;
        return;
    }

    unsigned int secondCounter = 1;
    while (running) {
        std::unique_lock<std::mutex> lock(mtx);

        std::cout << secondCounter << " Seconds" << ":\n";
        printStatus();

        bool formed = false;
        while (canFormParty()) {
            auto instanceId = findAvailableInstance();
            if (instanceId == -1) break;

            formParty(instanceId);
            formed = true;
        }

        if (!formed) {
            if (!tankQueue.size() || !healerQueue.size() || dpsQueue.size() < 3) {
                if (std::all_of(instances.begin(), instances.end(),
                    [](const Instance& i) { return !i.active; })) {
                    running = false;
                }
                else {
                    cv.wait_for(lock, std::chrono::seconds(1));
                }
            }
            else {
                cv.wait_for(lock, std::chrono::seconds(1));
            }
        }

        secondCounter++;
    }

    printFinalSummary();
}

bool DungeonQueueSystem::areAllInstancesEmpty() const {
    return std::none_of(instances.begin(), instances.end(),
        [](const Instance& i) { return i.active; });
}

void DungeonQueueSystem::printStatus() const {
    std::cout << "Current Instance Status:\n";
    for (unsigned int i = 0; i < n; i++) {
        std::cout << "  Instance " << i + 1 << ": "
            << (instances[i].active ? "active" : "empty") << "\n";
    }
    std::cout << "Queue Status:\n";
    std::cout << "  Tanks: " << tankQueue.size() << "\n";
    std::cout << "  Healers: " << healerQueue.size() << "\n";
    std::cout << "  DPS: " << dpsQueue.size() << "\n\n";
}

void DungeonQueueSystem::printFinalSummary() const {
    std::cout << "\n=== Final Summary ===\n";
    std::cout << std::left << std::setw(15) << "Instance"
        << std::setw(15) << "Parties Served"
        << "Total Time Served\n";

    for (unsigned int i = 0; i < n; i++) {
        std::cout << std::left << std::setw(15) << ("Instance " + std::to_string(i + 1))
            << std::setw(15) << instances[i].partiesServed
            << instances[i].totalTimeServed << " seconds\n";
    }

    std::cout << "\nUnassigned Players:\n";
    std::cout << "  Tanks: " << tankQueue.size() << "\n";
    std::cout << "  Healers: " << healerQueue.size() << "\n";
    std::cout << "  DPS: " << dpsQueue.size() << "\n";
}