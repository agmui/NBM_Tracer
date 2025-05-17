//
// Created by agmui on 5/13/25.
//

#include "Network.h"

#define TIMEOUT 1 // in ms

using namespace std;

int Network::sendMessage(vector<uint8_t> msg, int msgSock)
{
    size_t size = msg.size();
    std::cout << "Sending message of size = " << size << std::endl;

    //TODO: merge the two send calls to avoid 2 system calls
    if (send(msgSock, &size, sizeof(size_t), 0) != sizeof(size_t))
    {
        perror("Failed to send message size");
        return -1;
    }

    if (send(msgSock, msg.data(), size, 0) != size)
    {
        perror("Failed to send message");
        return -1;
    }

    for (unsigned char i : msg)
    {
        printf("%02x ", i);
    }
    printf("\n");

    printf("Success\n");
    return 0;
}

unique_ptr<Task> Network::waitForTask(int msgSock)
{
    size_t size;
    int bytes_expected;
    int bytes_received;

    printf("Receiving task from server\n");

    bytes_expected = sizeof(size_t);
    bytes_received = 0;
    while (bytes_received < bytes_expected)
    {
        int n = recv(msgSock, &size, sizeof(size_t), 0);
        if (n < 0)
        {
            continue;
        }
        else if (n == 0)
        {
            printf("Server closed\n");
            shutdown();
            exit(0);
        }
        bytes_received += n;
    }

    vector<uint8_t> buffer(size);
    size_t total = 0;
    while (total < size)
    {
        int n = recv(sock, buffer.data() + total, size - total, 0);
        if (n < 0)
        {
            continue;
        }
        else if (n == 0)
        {
            printf("Server closed\n");
            shutdown();
            exit(0);
        }
        total += n;
    }

    for (size_t i = 0; i < size; ++i)
    {
        printf("%02x ", buffer[i]);
    }
    printf("\n");

    printf("Received %zu bytes\n", total);
    unique_ptr<Task> task = TaskFactory::deserialize(buffer);
    return task;
}

void Network::waitForResult(int msgSock, Task& task)
{
    size_t size;
    int bytes_expected;
    int bytes_received = 0;

    printf("Receiving result from client\n");

    int received;
    while ((received = recv(msgSock, &size, sizeof(size_t), 0)) <= 0)
    {
    }

    printf("Got size %zu from %d bytes\n", size, received);

    vector<uint8_t> buffer(size);
    size_t total = 0;
    while (total < size)
    {
        size_t n = recv(msgSock, buffer.data() + total, size - total, 0);
        if (n <= 0)
            break;
        total += n;
    }

    for (size_t i = 0; i < size; ++i)
    {
        printf("%02x ", buffer[i]);
    }
    printf("\n");

    printf("Received %zu bytes\n", total);
    task.fillResults(buffer);
}

void *Network::getInAddr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

void Network::createHints(struct addrinfo *hints, int sock_type)
{
    memset(hints, 0, sizeof(struct addrinfo));
    hints->ai_family = AF_UNSPEC;
    hints->ai_socktype = sock_type;
    hints->ai_flags = AI_PASSIVE;
}

void Network::createAndBindSocket(struct addrinfo *servinfo, struct addrinfo **p)
{
    int yes = 1;
    struct addrinfo *temp;

    for (temp = servinfo; temp != NULL; temp = temp->ai_next)
    {
        if ((sock = socket(temp->ai_family, temp->ai_socktype, temp->ai_protocol)) == -1)
        {
            perror("server: socket\n");
            continue;
        }

        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1)
        {
            perror("server: setsockopt\n");
            exit(1);
        }

        struct timeval timeout = {TIMEOUT, 0};
        if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout) == -1)
        {
            perror("server: setsockopt\n");
            exit(1);
        }
        if (bind(sock, temp->ai_addr, temp->ai_addrlen) == -1)
        {
            perror("server: bind\n");
            close(sock);
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo);

    if (temp == NULL)
    {
        perror("server: failed to bind\n");
        exit(1);
    }

    *p = temp;

    if (listen(sock, 10) == -1)
    {
        perror("listen");
        exit(1);
    }
}

void Network::printServerInfo(struct addrinfo *p, char *port)
{
    char ipstr[INET6_ADDRSTRLEN];
    void *addr;
    int protocol;

    if (p->ai_family == AF_INET6)
    {
        struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
        addr = &(ipv6->sin6_addr);
        protocol = AF_INET6;
    }
    else
    {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
        addr = &(ipv4->sin_addr);
        protocol = AF_INET;
    }

    // Had issues with protocol not being AF_INET or AF_INET6, so defaulted to AF_INET
    if (inet_ntop(protocol, addr, ipstr, INET6_ADDRSTRLEN) == NULL)
    {
        perror("server: inet_ntop\n");
        printf("protocol %d\n", protocol);
        exit(1);
    }
    printf("Server on host %s is listening on port %s\n", ipstr, port);
}

int Network::serverAcceptConnection()
{
    struct sockaddr_storage clientAddr{};
    char s[INET6_ADDRSTRLEN];
    socklen_t sin_size;
    int msgSock;

    sin_size = sizeof(struct sockaddr_storage);
    if ((msgSock = accept(sock, (struct sockaddr *)&clientAddr, &sin_size)) == -1)
    {
        return -1;
    }

    inet_ntop(clientAddr.ss_family, getInAddr((struct sockaddr *)&clientAddr), s, sizeof s);
    printf("Received connection request from %s\n", s);
    printf("***************************************************\n");

    return msgSock;
}

void Network::performServerSetup(char *port)
{
    int status;

    createHints(&hints, SOCK_STREAM);

    if ((status = getaddrinfo(NULL, port, &hints, &servinfo)) != 0)
    {
        perror("server: getaddrinfo\n");
        exit(1);
    }

    createAndBindSocket(servinfo, &p);

    printServerInfo(p, port);
}

void Network::performClientSetup(char *ipString, char *port)
{
    int status;
    struct addrinfo *temp;

    createHints(&hints, SOCK_STREAM);

    if ((status = getaddrinfo(ipString, port, &hints, &servinfo)) != 0)
    {
        perror("client: getaddrinfo");
        exit(1);
    }

    for (temp = servinfo; temp != NULL; temp = temp->ai_next)
    {
        if ((sock = socket(temp->ai_family, temp->ai_socktype, temp->ai_protocol)) == -1)
        {
            perror("client: socket\n");
            continue;
        }

        if (connect(sock, temp->ai_addr, temp->ai_addrlen) == -1)
        {
            close(sock);
            perror("client: connect");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo);

    if (temp == NULL)
    {
        printf("client: failed to connect\n");
        exit(1);
    }
}

void Network::shutdown()
{
    close(sock);
}