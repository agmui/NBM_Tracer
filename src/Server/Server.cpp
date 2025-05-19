//
// Created by agmui on 5/13/25.
//

#include "Server.h"

void Server::start(char *port) {
    network.performServerSetup(port);
    generateTasks();
    threadpool.initThreads();
    threadpool.joinAllThreads(); // TODO: handle random clients connection/disconnection
    
}

//TODO: maybe should be moved to server_app
void Server::generateTasks() {
//    for (int i = 0; i < 2; i++) {
//        unique_ptr<MineBitCoin> newTask = make_unique<MineBitCoin>(i, i , i * i - i);
//        threadpool.addTask(std::move(newTask));
//    }


//    MuianRayTracer tracer;
//    tracer.init();
    printf("started adding tasks\n");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; ++j) {
            unique_ptr<RenderPixel> newTask = make_unique<RenderPixel>(i,j);
            threadpool.addTask(std::move(newTask));
        }
    }
    printf("finished adding tasks\n");
}
