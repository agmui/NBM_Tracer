//
// Created by agmui on 5/13/25.
//

#include "Threadpool.h"

int Threadpool::waitForClient(int tid) {
    int msgSock = network.serverAcceptConnection();

    m.lock();
    while (!tasks.empty()) {
        unique_ptr<Task> t(tasks.front().release());
        tasks.pop();
        m.unlock();

        network.sendTask(TaskFactory::serialize(*t), msgSock);//TODO: fix t
        network.waitForResult(t->getResult(), tid, msgSock);

        m.lock();
    }
    m.unlock();

    close(msgSock);
    network.shutdown();

    return 0;
}

void Threadpool::initThreads() {
    m.lock();
    for (int i = 0; i < numThreads; ++i) {
        int tid = freeThreads.front();
        freeThreads.pop();
        workingThreads.push_back(tid);
        threads[i] = thread([this, tid]() { waitForClient(tid); });
    }
    m.unlock();

}
