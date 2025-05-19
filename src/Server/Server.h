//
// Created by agmui on 5/13/25.
//

#ifndef NBM_TRACER_SERVER_H
#define NBM_TRACER_SERVER_H

#include "Network.h"
#include "Threadpool.h"
#include "Tasks/RenderPixel.h"
#include "Tasks/BatchedRender.h"

class Server
{

public:
    Server(Network &network, Threadpool &threadpool) : network(network), threadpool(threadpool) {};

    void start(char *port);
    void generateTasks();
private:
    Network &network;
    Threadpool &threadpool;
};

#endif // NBM_TRACER_SERVER_H
