#ifndef DUNGEON_QUEUE_SYSTEM_H
#define DUNGEON_QUEUE_SYSTEM_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <random>
#include <atomic>

class DungeonQueueSystem {
private:
    unsigned int n, t1, t2;
    std::queue<unsigned int> tankQueue, healerQueue, dpsQueue;

    struct Instance {
        bool active;
        unsigned int partiesServed;
        unsigned int totalTimeServed;
        unsigned int tankId;
        unsigned int healerId;
        std::vector<unsigned int> dpsIds;

        Instance();
        void reset();
    };

    std::vector<Instance> instances;
    std::mutex mtx;
    std::condition_variable cv;
    std::atomic<bool> running;
    std::mt19937 rng;
    std::uniform_int_distribution<unsigned int> timeDist;

    unsigned int findAvailableInstance();
    bool canFormParty() const;
    void formParty(unsigned int instanceId);

public:
    DungeonQueueSystem(unsigned int n, unsigned int t, unsigned int h,
        unsigned int d, unsigned int t1, unsigned int t2);

    void start();
    bool areAllInstancesEmpty() const;
    void printStatus() const;
    void printFinalSummary() const;
};

#endif#pragma once
