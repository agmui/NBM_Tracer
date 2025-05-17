//
// Created by agmui on 5/13/25.
//

#include "Threadpool.h"

int Threadpool::waitForClient(int tid)
{
    int msgSock = -1;
    while (msgSock == -1)
    {
        networkLock.lock();
        msgSock = network.serverAcceptConnection();
        numConnected++;
        networkLock.unlock();

    }

    tasksLock.lock();
    while (!tasks.empty())
    {
        unique_ptr<Task> t(tasks.front().release()); // jank transfer of ownership
        tasks.pop();
        tasksLock.unlock();

        network.sendMessage(t->serialize(), msgSock); // TODO: fix t
        network.waitForResult(msgSock, *t);

        auto result = t->getResult();
        result->printResult();
        results.push_back(std::move(result));

        tasksLock.lock();
    }
    tasksLock.unlock();

    close(msgSock);

    return 0;
}

void Threadpool::addTask(unique_ptr<Task> task) {
    tasks.emplace(std::move(task));
}

void Threadpool::initThreads()
{
    printf("Initializing threads...\n");
    tasksLock.lock();
    for (int i = 0; i < numThreads; ++i)
    {
        // int tid = freeThreads.front();
        // freeThreads.pop();
        // workingThreads.push_back(tid);
        threads[i] = thread([this, i]()
                            { waitForClient(i); });
    }
    tasksLock.unlock();
}

void Threadpool::joinAllThreads()
{
    threads[0].join();
//    for (int i = 0; i < numThreads; ++i)
//    {
//        threads[i].join();
//    }
    network.shutdown();
}
