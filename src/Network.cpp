//
// Created by agmui on 5/13/25.
//

#include "Network.h"

#define TIMEOUT 1 // in ms

using namespace std;

int Network::sendTask(vector<uint8_t> &&msg, int msgSock) {
    printf("Attempting to send task\n");
    if (send(msgSock, &msg, msg.size(), 0) != msg.size()) {
        perror("Failed to send task");
        return -1;
    }
    printf("Success\n");
    return 0;
}

unique_ptr<Task> Network::waitForTask(int msgSock) {
    printf("Receiving result from server\n");

    size_t size;
    while (recv(msgSock, &size, sizeof(size_t), 0) < 0) {

    }
    vector<uint8_t> buff;
    buff.resize(size);//TODO: optimize
    while (recv(msgSock, &buff[0], size, 0) < 0) {
//        printf("Client %d still working...\n", tid);
    }
    printf("Success\n");
    return TaskFactory::deserialize(buff);
}

int Network::waitForResult(Result *result, int tid, int msgSock) {
    printf("Receiving result from client\n");
    vector<uint8_t> buff;
    buff.resize(result->size());//TODO: optimize
    while (recv(msgSock, &buff[0], result->size(), 0) < 0) {
        printf("Client %d still working...\n", tid);
    }
    result->fill(buff);
    printf("Success\n");
    return 0;
}

void *Network::getInAddr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *) sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *) sa)->sin6_addr);
}

void Network::createHints(struct addrinfo *hints, int sock_type) {
    memset(hints, 0, sizeof(struct addrinfo));
    hints->ai_family = AF_UNSPEC;
    hints->ai_socktype = sock_type;
    hints->ai_flags = AI_PASSIVE;
}

void Network::createAndBindSocket(struct addrinfo *servinfo, struct addrinfo **p) {
    int yes = 1;
    struct addrinfo *temp;

    for (temp = servinfo; temp != NULL; temp = temp->ai_next) {
        if ((sock = socket(temp->ai_family, temp->ai_socktype, temp->ai_protocol)) == -1) {
            perror("server: socket\n");
            continue;
        }

        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
            perror("server: setsockopt\n");
            exit(1);
        }

        struct timeval timeout = {TIMEOUT, 0};
        if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout) == -1) {
            perror("server: setsockopt\n");
            exit(1);
        }
        if (bind(sock, temp->ai_addr, temp->ai_addrlen) == -1) {
            perror("server: bind\n");
            close(sock);
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo);

    if (temp == NULL) {
        perror("server: failed to bind\n");
        exit(1);
    }

    *p = temp;

    if (listen(sock, 10) == -1) {
        perror("listen");
        exit(1);
    }
}

void Network::printServerInfo(struct addrinfo *p, char *port) {
    char ipstr[INET6_ADDRSTRLEN];
    void *addr;
    int protocol;

    if (p->ai_family == AF_INET6) {
        struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *) p->ai_addr;
        addr = &(ipv6->sin6_addr);
        protocol = AF_INET6;
    } else {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *) p->ai_addr;
        addr = &(ipv4->sin_addr);
        protocol = AF_INET;
    }

    // Had issues with protocol not being AF_INET or AF_INET6, so defaulted to AF_INET
    if (inet_ntop(protocol, addr, ipstr, INET6_ADDRSTRLEN) == NULL) {
        perror("server: inet_ntop\n");
        printf("protocol %d\n", protocol);
        exit(1);
    }
    printf("Server on host %s is listening on port %s\n", ipstr, port);
}

int Network::serverAcceptConnection() {
    struct sockaddr_storage clientAddr {};
    char s[INET6_ADDRSTRLEN];
    socklen_t sin_size;
    int msgSock;

    sin_size = sizeof(struct sockaddr_storage);
    if ((msgSock = accept(sock, (struct sockaddr *) &clientAddr, &sin_size)) == -1) {
        perror("server: accept");
        return -1;
    }

    inet_ntop(clientAddr.ss_family, getInAddr((struct sockaddr *) &clientAddr), s, sizeof s);
    printf("Received connection request from %s\n", s);
    printf("***************************************************\n");

    return msgSock;
}

void Network::performServerSetup(char *port) {
    int status;

    createHints(&hints, SOCK_STREAM);

    if ((status = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
        perror("server: getaddrinfo\n");
        exit(1);
    }

    createAndBindSocket(servinfo, &p);

    printServerInfo(p, port);
}

void Network::performClientSetup(char *ipString, char *port) {
    int status;
    struct addrinfo *temp;

    createHints(&hints, SOCK_STREAM);

    if ((status = getaddrinfo(ipString, port, &hints, &servinfo)) != 0) {
        perror("client: getaddrinfo");
        exit(1);
    }

    for (temp = servinfo; temp != NULL; temp = temp->ai_next) {
        if ((sock = socket(temp->ai_family, temp->ai_socktype, temp->ai_protocol)) == -1) {
            perror("client: socket\n");
            continue;
        }

        if (connect(sock, temp->ai_addr, temp->ai_addrlen) == -1) {
            close(sock);
            perror("client: connect");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo);

    if (temp == NULL) {
        printf("client: failed to connect\n");
        exit(1);
    }
}

void Network::shutdown() {
    close(sock);
}