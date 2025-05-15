//
// Created by agmui on 5/13/25.
//

#include "Server.h"

void Server::start(char *port) {
    network->performServerSetup(port);
    threadpool->initThreads();
    threadpool->joinAllThreads();
}
