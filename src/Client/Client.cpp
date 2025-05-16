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
    unique_ptr<Task> task = network.waitForTask(network.sock);
    printf("Task id: %d\n", task->getId());
    
    unique_ptr<Result> result = createResult(task->resultType);
    task->doTask(result);
    network.sendMessage(result->serialize(), network.sock);
}