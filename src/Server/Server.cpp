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
//    for (int i = 0; i < RES; i++) {
//        for (int j = 0; j < RES; ++j) {
//            unique_ptr<RenderPixel> newTask = make_unique<RenderPixel>(j,i);
//            threadpool.addTask(std::move(newTask));
//        }
//    }

    int divFactor = 100;
    int widthJumpSize = RES / divFactor;
    int heightJumpSize = RES / divFactor;
    int widthCutoff = RES % widthJumpSize;
    int heightCutoff = RES % heightJumpSize;
    for (int i = 0; i < RES; i+=widthJumpSize) {
        for (int j = 0; j < RES; j+=heightJumpSize) {
            unique_ptr<BatchedRender> newTask = make_unique<BatchedRender>(j, i, widthJumpSize, heightJumpSize);
            threadpool.addTask(std::move(newTask));
        }
    }
//    unique_ptr<BatchedRender> rightCutoff = make_unique<BatchedRender>(0,RES-widthCutoff, widthCutoff, RES);
//    threadpool.addTask(std::move(rightCutoff));
//    unique_ptr<BatchedRender> bottomCutoff = make_unique<BatchedRender>(RES - height, 0, RES-widthCutoff, heightCutoff);
//    threadpool.addTask(std::move(bottomCutoff));
    printf("finished adding tasks\n");
}
