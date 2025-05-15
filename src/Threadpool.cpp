//
// Created by agmui on 5/13/25.
//

#include "Threadpool.h"

int Threadpool::waitForClient(int tid) {
    int msgSock = network.serverAcceptConnection();

    char arr[1024];
    Result *result = (Result *)arr;

    while(!tasks.empty()) {
        network.sendTask(tasks.front(), msgSock);
        tasks.pop();
        network.waitForResult(result, 0, msgSock);
    }

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
