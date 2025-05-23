//
// Created by agmui on 5/13/25.
//

#include "Threadpool.h"

int Threadpool::waitForClient(int tid, vector<pair<const char*,FILE*>> files)
{
    int msgSock = -1;
    networkLock.lock();
    while (msgSock == -1 && !finished)
    {
//        networkLock.lock();
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
//        networkLock.unlock();
    }
    if (finished){
        networkLock.unlock();
        return 0;
    }
    networkLock.unlock();

    // ========================== sending files ======================================
    for (auto p : files) {
        printf("sending file...\n");
        if(network.sendFile(p.second, p.first, msgSock) != 0){
            printf("[WARNING] client disconnected\n");
            return 1;
        }
    }

    // ========================== sending RES ======================================
    printf("sending RES\n");
    vector<uint8_t> v;
    v.resize(sizeof(RES));
    memcpy(v.data(), &RES, sizeof(RES));
    network.sendMessage(v, msgSock);

    // ========================== waiting for start ======================================
    {
        unique_lock<mutex> lock(startTasksLock);
        cv.wait(lock, [this]{return start;});
    }//lock released here

//    while(!start){ //TODO: use condvar
//        sleep(0.1);
//    }

    tasksLock.lock();
    while (!finished)
    {
        unique_ptr<Task> t(tasks.front().release()); // jank transfer of ownership
        tasks.pop();
        tasksLock.unlock();

        vector<uint8_t> taskData = t->serialize();
        taskData.emplace(taskData.begin(), t->getTaskIndex()); //TODO: this is jank
        printf("sending task index: %d\n", t->getTaskIndex());
        if(network.sendMessage(taskData, msgSock)!=0){ // TODO: fix t
            printf("[WARNING] failed to send msg\n");
            tasksLock.lock();
            tasks.push(std::move(t));
            tasksLock.unlock();
            return 1;
        }
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

    if (msgSock != -1) {
        close(msgSock);
    }

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
                            {
            int error = 0;
            do{
                error = waitForClient(i, files);
                if(error != 0)
                    printf("[WARNING] waitForClient() Crashed retrying\n");
            }while(error != 0);
                            });
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
    printf("finished joining threads\n");
    network.shutdown();
}
