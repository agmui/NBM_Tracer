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

#define MAX_NUM_THREADS 50

using namespace std;

class Threadpool //TODO: refactor to call it NetworkedThreadPool
{
public:
    Threadpool(int numThreads, Network &network) : numThreads(numThreads), network(network) {}

    void initThreads(const vector<pair<const char*,FILE*>>& files);
    void joinAllThreads();

    void startThreads(){ start = true; }

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
    mutex resultsLock;
    mutex networkLock;
    vector<unique_ptr<Result>> results;

    int numConnected = 0; //TODO: find better system
    bool start = false;
    bool finished = false;

    int waitForClient(int tid, vector<pair<const char*,FILE*>> fptr);
};

#endif // NBM_TRACER_THREADPOOL_H
