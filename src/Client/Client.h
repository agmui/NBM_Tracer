//
// Created by agmui on 5/13/25.
//

#ifndef NBM_TRACER_CLIENT_H
#define NBM_TRACER_CLIENT_H

#include <memory>
#include "Network.h"
#include "Results/Result.h"

class Client {
public:
    void start(char *ipString, char *port);
    unique_ptr<Result> createResult(ResultTypes resultType);
    
private:
    Network network;
};


#endif //NBM_TRACER_CLIENT_H
