//
// Created by agmui on 5/13/25.
//

#include "Client/Client.h"
//#include "TracerInterface.h"

#include "muian_raytracer/src/RayTracer.h"

int main(int argc, char **argv) {
    printf("client!\n");
    sleep(2);
    const int ip_max_len = 16;
    const int port_max_len = 5;
    if (argc != 3 || strlen(argv[1]) > ip_max_len || strlen(argv[2]) > port_max_len) {
        printf("ERROR: invalid arguments\n");
        exit(1);
    }

    Client client{};
    client.start(argv[1], argv[2]);
    return 0;
}