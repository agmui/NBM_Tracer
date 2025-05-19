//
// Created by agmui on 5/13/25.
//

#ifndef NBM_TRACER_NETWORK_H
#define NBM_TRACER_NETWORK_H

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cctype>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cstring>

#include "Tasks/Task.h"
#include "Results/Result.h"
#include "Tasks/MineBitCoin.h"
#include "Tasks/RenderPixel.h"
#include "Tasks/BatchedRender.h"

using namespace std;


class Network
{

public:
    int sendMessage(vector<uint8_t> msg, int msgSock);
    shared_ptr<Task> waitForTask(int msgSock);
    void waitForResult(int msgSock, Task& task);
    void *getInAddr(struct sockaddr *sa);
    void createHints(struct addrinfo *hints, int sock_type);
    void createAndBindSocket(struct addrinfo *servinfo, struct addrinfo **p);
    void printServerInfo(struct addrinfo *p, char *port);
    int serverAcceptConnection();
    void init(){
        MineBitCoin::setTaskIndex(0);
        RenderPixel::setTaskIndex(1);
        BatchedRender::setTaskIndex(2);
        taskList = {
                make_shared<MineBitCoin>(-1, -1, -1),
                make_shared<RenderPixel>(-1,-1),
                make_shared<BatchedRender>(-1,-1,0,0)
        };
    }
    void performServerSetup(char *port);
    void performClientSetup(char *ipString, char *port);
    void shutdown();

    int sock;

private:
    struct addrinfo hints;
    struct addrinfo *servinfo;
    struct addrinfo *p;
    vector<shared_ptr<Task>> taskList;

};

#endif // NBM_TRACER_NETWORK_H
