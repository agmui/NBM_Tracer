//
// Created by agmui on 5/13/25.
//

#include "Threadpool.h"

int Threadpool::waitForClient(int tid)
{
    int msgSock = -1;
    while (msgSock == -1)
    {
        m.lock();
        msgSock = network->serverAcceptConnection();
        m.unlock();

    }


    // m.lock();
    // while (!tasks.empty())
    // {
    //     unique_ptr<Task> t(tasks.front().release());
    //     tasks.pop();
    //     m.unlock();

    //     network.sendTask(TaskFactory::serialize(*t), msgSock); // TODO: fix t
    //     network.waitForResult(t->getResult(), tid, msgSock);

    //     m.lock();
    // }
    // m.unlock();
    printf("Doing stuff on tid %d\n", tid);

    close(msgSock);

    return 0;
}

void Threadpool::initThreads()
{
    printf("Initializing threads...\n");
    m.lock();
    for (int i = 0; i < numThreads; ++i)
    {
        // int tid = freeThreads.front();
        // freeThreads.pop();
        // workingThreads.push_back(tid);
        threads[i] = thread([this, i]()
                            { waitForClient(i); });
    }
    m.unlock();
}

void Threadpool::joinAllThreads()
{
    for (int i = 0; i < numThreads; ++i)
    {
        threads[i].join();
    }
}
