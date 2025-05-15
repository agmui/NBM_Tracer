//
// Created by agmui on 5/13/25.
//

#include <bits/stdc++.h>
#include "TracerInterface.h"
#include "Server/Server.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: ./server <port>");
        exit(1);
    }

    Network network;
    Threadpool threadpool(2, &network);
    Server server(&network, &threadpool);
    server.start(argv[1]);
}