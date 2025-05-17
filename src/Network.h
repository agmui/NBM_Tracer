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
#include "Tasks/TaskFactory.h"
#include "Tasks/MineBitCoin.h"

using namespace std;


class Network
{

public:
    int sendMessage(vector<uint8_t> msg, int msgSock);
    unique_ptr<Task> waitForTask(int msgSock);
    void waitForResult(int msgSock, Task& task);
    void *getInAddr(struct sockaddr *sa);
    void createHints(struct addrinfo *hints, int sock_type);
    void createAndBindSocket(struct addrinfo *servinfo, struct addrinfo **p);
    void printServerInfo(struct addrinfo *p, char *port);
    int serverAcceptConnection();
    void performServerSetup(char *port);
    void performClientSetup(char *ipString, char *port);
    void shutdown();

    int sock;

private:
    struct addrinfo hints;
    struct addrinfo *servinfo;
    struct addrinfo *p;

};

#endif // NBM_TRACER_NETWORK_H
