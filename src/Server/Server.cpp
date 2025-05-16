//
// Created by agmui on 5/13/25.
//

#include "Server.h"

void Server::start(char *port) {
    network->performServerSetup(port);
    generateTasks();
    threadpool->initThreads();
    threadpool->joinAllThreads();
    
}

void Server::generateTasks() {
    for (int i = 0; i < 10; i++) {
        unique_ptr<MineBitCoin> newTask = make_unique<MineBitCoin>(i, i, i * i - i);
        threadpool->addTask(move(newTask));
    }
}
