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
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bits/stdc++.h>

#include "Tasks/Task.h"
#include "Result.h"

class Network {

public:
    int sendTask(Task &msg, int msgSock);
    int waitForResult(Result &msg, int tid, int msgSock);
    void *getInAddr(struct sockaddr *sa);
    void createHints(struct addrinfo *hints, int sock_type);
    void createAndBindSocket(struct addrinfo *servinfo, struct addrinfo **p);
    void printServerInfo(struct addrinfo *p, char *port);
    int serverAcceptConnection();
    void performServerSetup(int sock_type, char *port, struct addrinfo *hints, struct addrinfo *servinfo, struct addrinfo *p);
    void performClientSetup(int sock_type, char *ipString, char *port, struct addrinfo *hints, struct addrinfo *servinfo, struct addrinfo **p);
    void shutdown();

private:
    int sock;
};


#endif //NBM_TRACER_NETWORK_H
