//
// Created by agmui on 5/13/25.
//

#ifndef NBM_TRACER_CLIENT_H
#define NBM_TRACER_CLIENT_H

#include "Network.h"

class Client {
public:
    void start(char *ipString, char *port);
private:
    Network network;
};


#endif //NBM_TRACER_CLIENT_H
