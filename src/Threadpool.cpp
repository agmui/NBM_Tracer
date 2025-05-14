//
// Created by agmui on 5/13/25.
//

#include "Threadpool.h"

int Threadpool::wait_for_client() {
    int msgSock = network.serverAcceptConnection();

    char arr[1024];
    Result *result = (Result *)arr;

    while(!tasks.empty()){
        network.sendTask(tasks.front(), msgSock);
        tasks.pop();
        network.waitForResult(result, 0, msgSock);
    }

    close(msgSock);
    network.shutdown();

    return 0;
}
