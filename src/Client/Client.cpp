//
// Created by agmui on 5/13/25.
//

#include "Client.h"


void Client::start(char *ipString, char *port)
{
    network.performClientSetup(ipString, port);
    while (1) //TODO: have better stop condition
    {
        shared_ptr<Task> task = network.waitForTask(network.sock);
        printf("Task id: %d\n", task->getId());

        Result& r = task->doTask();
        printf("finished task\n");
//        sleep(2);
        auto tmp = r.serialize();
        network.sendMessage(tmp, network.sock);
    }
}