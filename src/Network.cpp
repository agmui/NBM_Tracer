//
// Created by agmui on 5/13/25.
//

#include "Network.h"


using namespace std;

int Network::sendFile(FILE *fp, const char* fileName, int msgSock) {
    int bytes_to_send;
    char send_buf[FILE_PKT_SIZE];
    int total_sent = 0;


    // ============== sending filename size + file size =============
    fseek(fp, 0L, SEEK_END); //seek to the end of the file
    long sz = ftell(fp); // ask for the posistion
    fseek(fp, 0L, SEEK_SET); // seek back to top of file
    size_info data = {strlen(fileName),sz};
    printf("sending data | filenameSize: %zu sz: %ld\n", data.filenameSize, data.sz);
    if(send(msgSock, &data, sizeof(size_info), 0) != sizeof(size_info))
        perror("server: send file size");


    // ============== sending file name =============
    printf("sending file name: %s\n",fileName);
    if(send(msgSock, fileName, data.filenameSize, 0) != data.filenameSize)
        perror("server: send filename");

    // ============== sending file itself =============
    //TODO: pass char buff instead of syscall of fread
    while ((bytes_to_send = fread(send_buf, 1, FILE_PKT_SIZE, fp)) > 0) {
        if (send(msgSock, send_buf, bytes_to_send, 0) != bytes_to_send)
        {
            perror("server: send file");
            break;
        }
        total_sent += bytes_to_send;
    }

    printf("Size of file sent %d\n", total_sent);
    return 0; //TODO: return error code
}

int Network::recvFile(int msgSock) {
    int total_received = 0;
    size_t bytes_received;
    char recv_buf[FILE_PKT_SIZE];
    FILE *fp;

    while (1) {

        // ============== receiving filename size + file size =============
        size_info data{};
        if (recv(msgSock, &data, sizeof(size_info), 0) < 0) {
            perror("client: receive");
            break;
        }
        printf("received data | filenameSize: %zu sz: %ld\n", data.filenameSize, data.sz);

        // ============== receiving filename =============
        filenames.emplace_back("");
        string &filename = filenames.back();
        filename.resize(data.filenameSize); //TODO: check if we still need to add strlen
        if (recv(msgSock, filename.data(), data.filenameSize, 0) < 0) {
            perror("client: receive");
            break;
        }

        cout << "filename: " << filename << endl;

        fp = fopen(filename.c_str(), "wb");
        if (fp) {
            // ============== receiving file itself =============
            printf("Now writing file...\n");
            while (total_received < data.sz) {
                size_t bytesToRead = (data.sz - total_received < FILE_PKT_SIZE)? data.sz-total_received: sizeof(recv_buf);
                bytes_received = recv(msgSock, recv_buf, bytesToRead, 0);
                total_received += bytes_received;
                if (fwrite(recv_buf, 1, bytes_received, fp) != bytes_received)
                {
                    perror("client: fwrite\n");
                    break;
                }
            }

            fclose(fp);
            printf("File written successfully\n");
            break;
        } else {
            fclose(fp);
            printf("Failed to open file %s\n", filename.c_str());
        }
    }
    return 0;
}

int Network::sendMessage(vector<uint8_t> msg, int msgSock)
{
    size_t size = msg.size();
    std::cout << "Sending message of msgSize = " << size << std::endl;

    if (send(msgSock, msg.data(), size, 0) != size)
    {
        perror("Failed to send message");
        return -1;
    }

//    for (unsigned char i : msg)
//    {
//        printf("%02x ", i);
//    }
//    printf("\n");

    printf("Success\n");
    return 0;
}

shared_ptr<Task> Network::waitForTask(int msgSock)
{
    uint8_t taskIndex;
    size_t bytes_expected = sizeof(uint8_t);
    int bytes_received = 0;

    printf("Receiving task from server\n");

    while (bytes_received < bytes_expected)
    {
        int n = recv(msgSock, &taskIndex, bytes_expected, 0);
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


    shared_ptr<Task> t = taskList.at(taskIndex);
    size_t size = t->msgSize();
    printf("got task index: %d, expected size: %zu\n", taskIndex, size);
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

//    for (size_t i = 0; i < size; ++i)
//    {
//        printf("%02x ", buffer[i]);
//    }
//    printf("\n");

    printf("Received %zu bytes\n", total);
    t->deserialize(buffer);
    return t;
}

void Network::waitForResult(int msgSock, Task& task)
{
    size_t size = task.getResult()->size();
    int bytes_expected;
    int bytes_received = 0;

    printf("Receiving result from client\n");

//    int received;
//    while ((received = recv(msgSock, &size, sizeof(size_t), 0)) <= 0)
//    {
//    }
//
//    printf("Got msgSize %zu from %d bytes\n", size, received);

    vector<uint8_t> buffer(size);
    size_t total = 0;
    while (total < size)
    {
        size_t n = recv(msgSock, buffer.data() + total, size - total, 0);
        if (n <= 0)
            break;
        total += n;
    }

//    for (size_t i = 0; i < size; ++i)
//    {
//        printf("%02x ", buffer[i]);
//    }
//    printf("\n");

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
    serverInit();
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


