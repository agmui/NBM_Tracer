//
// Created by agmui on 5/13/25.
//

#ifndef NBM_TRACER_THREADPOOL_H
#define NBM_TRACER_THREADPOOL_H

#include <memory>
#include <thread>
#include <thread>
#include <queue>
#include <list>

#include "Network.h"
#include "Tasks/Task.h"
#include "Tasks/TaskFactory.h"

#define MAX_NUM_THREADS 50

using namespace std;

class Threadpool //TODO: refactor to call it NetworkedThreadPool
{
public:
    Threadpool(int numThreads, Network &network) : numThreads(numThreads), network(network) {}

    void initThreads();
    void joinAllThreads();

    void addTask(unique_ptr<Task> task);

    vector<unique_ptr<Result>>& getResults(){
        return results;
    }

private:
    Network &network;

    int numThreads;
    //    int threadIds[numThreads];
    thread threads[MAX_NUM_THREADS]; // TODO: use vec
    list<int> workingThreads;
    queue<int> freeThreads;
    queue<unique_ptr<Task>> tasks;
    mutex tasksLock;
    mutex networkLock;
    vector<unique_ptr<Result>> results;

    int numConnected = 0; //TODO: find better system

    int waitForClient(int tid);
};

#endif // NBM_TRACER_THREADPOOL_H
