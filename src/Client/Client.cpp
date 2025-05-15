//
// Created by agmui on 5/13/25.
//

#include "Client.h"

void Client::start(char *ipString, char *port) {
    network.performClientSetup(ipString, port);
}