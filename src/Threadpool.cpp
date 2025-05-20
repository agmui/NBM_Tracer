//
// Created by agmui on 5/13/25.
//

#include "Threadpool.h"

int Threadpool::waitForClient(int tid, vector<pair<const char*,FILE*>> files)
{
    int msgSock = -1;
    while (msgSock == -1)
    {
        networkLock.lock();
        if (finished){ //TODO: use condvars
            networkLock.unlock();
            return 0;
        }
        msgSock = network.serverAcceptConnection();
        if (finished){
            networkLock.unlock();
            return 0;
        }
        numConnected++;
        networkLock.unlock();

    }

    for (auto p : files) {
        printf("sending file...\n");
        network.sendFile(p.second, p.first, msgSock);
    }

    while(!start){ //TODO: use condvar
        sleep(0.1);
    }

    tasksLock.lock();
    while (!finished)
    {
        unique_ptr<Task> t(tasks.front().release()); // jank transfer of ownership
        tasks.pop();
        tasksLock.unlock();

        vector<uint8_t> taskData = t->serialize();
        taskData.emplace(taskData.begin(), t->getTaskIndex()); //TODO: this is jank
        network.sendMessage(taskData, msgSock); // TODO: fix t
        network.waitForResult(msgSock, *t);

        auto result = t->getResult();
        result->printResult();
        resultsLock.lock();
        results.push_back(std::move(result));
        resultsLock.unlock();

        tasksLock.lock();
        finished = tasks.empty();
    }
    tasksLock.unlock();

    close(msgSock);

    return 0;
}

void Threadpool::addTask(unique_ptr<Task> task) {
    tasks.emplace(std::move(task));
}

void Threadpool::initThreads(const vector<pair<const char*,FILE *>>& files)
{
    printf("Initializing threads...\n");
    tasksLock.lock();

    for (int i = 0; i < numThreads; ++i)
    {
        // int tid = freeThreads.front();
        // freeThreads.pop();
        // workingThreads.push_back(tid);
        threads[i] = thread([this, i, files]()
                            { waitForClient(i, files); });
    }
    tasksLock.unlock();
}

void Threadpool::joinAllThreads()
{
//    threads[0].join();
    //FIXME: it throws this error when not all threads join before the program quits
    // terminate called without an active exception
    for (int i = 0; i < numThreads; ++i)
    {
        threads[i].join();
    }
    network.shutdown();
}
