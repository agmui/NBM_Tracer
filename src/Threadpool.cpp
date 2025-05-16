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

    m.lock();
    while (!tasks.empty())
    {
        unique_ptr<Task> t(tasks.front().release());
        tasks.pop();
        m.unlock();

        network->sendMessage(t->serialize(), msgSock); // TODO: fix t
        unique_ptr<Result> result = network->waitForResult(msgSock);
        BitCoinResult *bitCoinResult = static_cast<BitCoinResult *>(result.get());
        printf("Server got result %d from task id %d\n", bitCoinResult->sum, bitCoinResult->id);

        m.lock();
    }
    m.unlock();

    close(msgSock);

    return 0;
}

void Threadpool::addTask(unique_ptr<Task> task) {
    tasks.emplace(move(task));
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
    network->shutdown();
}
