//
// Created by agmui on 5/13/25.
//

#include "Threadpool.h"

int Threadpool::wait_for_client() {
    int msg_sock = network.serverAcceptConnection();

    while(!tasks.empty()){
        network.send(tasks.pop());
        result();
    }

    close(msg_sock);
    network.shutdown();
}
