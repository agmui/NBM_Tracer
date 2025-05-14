//
// Created by agmui on 5/13/25.
//

#ifndef NBM_TRACER_THREADPOOL_H
#define NBM_TRACER_THREADPOOL_H

#include <bits/stdc++.h>

#include "Network.h"
#include "Tasks/Task.h"

using namespace std;

class Threadpool {
    Network network;
    int numThreads;
//    int threadIds[numThreads];
    thread threads[2];
    queue<int> workingThreads;

    queue<Task> tasks;

    int wait_for_client();

};


#endif //NBM_TRACER_THREADPOOL_H
