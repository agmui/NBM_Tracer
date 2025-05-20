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

#define FILE_PKT_SIZE 64
#define TIMEOUT 1 // in ms

using namespace std;


class Network
{

public:
    int sendFile(FILE *fp, const char* fileName, int msgSock);
    int recvFile(int msgSock);
    int sendMessage(vector<uint8_t> msg, int msgSock);
    shared_ptr<Task> waitForTask(int msgSock);
    void waitForResult(int msgSock, Task& task);
    void *getInAddr(struct sockaddr *sa);
    void createHints(struct addrinfo *hints, int sock_type);
    void createAndBindSocket(struct addrinfo *servinfo, struct addrinfo **p);
    void printServerInfo(struct addrinfo *p, char *port);
    int serverAcceptConnection();
    void serverInit(){
        MineBitCoin::setTaskIndex(0);
        RenderPixel::setTaskIndex(1);
        BatchedRender::setTaskIndex(2);
    }
    void clientInit(){

        printf("in clientInit filename:%s\n", filenames[0].c_str());
        taskList = {
                make_shared<MineBitCoin>(-1, -1, -1),
                make_shared<RenderPixel>(-1,-1, filenames[0].c_str()),
                make_shared<BatchedRender>(-1,-1,0,0, filenames[0].c_str())
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
    struct size_info{
        size_t filenameSize;
        long sz;
    };
    vector<string> filenames;

};

#endif // NBM_TRACER_NETWORK_H
