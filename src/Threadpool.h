//
// Created by agmui on 5/13/25.
//

#ifndef NBM_TRACER_THREADPOOL_H
#define NBM_TRACER_THREADPOOL_H

#include <bits/stdc++.h>

#include "Network.h"
#include "Tasks/Task.h"
#include "Tasks/TaskFactory.h"

using namespace std;

class Threadpool
{
public:
    Threadpool(int numThreads, Network *network) : numThreads(numThreads), network(network) {}

    void initThreads();
    void joinAllThreads();

    void addTask(unique_ptr<Task> task);

private:
    Network *network;

    int numThreads;
    //    int threadIds[numThreads];
    thread threads[2];
    list<int> workingThreads;
    queue<int> freeThreads;
    queue<unique_ptr<Task>> tasks;
    mutex tasksLock;
    mutex networkLock;

    int waitForClient(int tid);
};

#endif // NBM_TRACER_THREADPOOL_H
