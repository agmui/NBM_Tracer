//
// Created by agmui on 5/13/25.
//

#include "Client.h"


void Client::start(char *ipString, char *port)
{
    network.performClientSetup(ipString, port);
    network.recvFile(network.sock);//TODO: this is jank
    network.recvFile(network.sock);
    network.getRES(network.sock);

    network.clientInit();
    while (1) //TODO: have better stop condition
    {
        shared_ptr<Task> task = network.waitForTask(network.sock);

        Result& r = task->doTask();
//        printf("finished task\n");
        auto tmp = r.serialize();
        network.sendMessage(tmp, network.sock);
    }
}