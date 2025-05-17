//
// Created by agmui on 5/13/25.
//

#include "Client.h"

unique_ptr<Result> Client::createResult(ResultTypes resultType)
{
    switch (resultType)
    {
    case BitCoinResult_:
        return make_unique<BitCoinResult>(-1, 0);
    default:
        // TODO: throw an error
        return nullptr;
    }
}

void Client::start(char *ipString, char *port)
{
    network.performClientSetup(ipString, port);
    while (1) //TODO: have better stop condition
    {
        unique_ptr<Task> task = network.waitForTask(network.sock);
        printf("Task id: %d\n", task->getId());

        Result& r = task->doTask();
        printf("finished task\n");
//        sleep(2);
        auto tmp = r.serialize();
        network.sendMessage(tmp, network.sock);
    }
}