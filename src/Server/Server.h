//
// Created by agmui on 5/13/25.
//

#ifndef NBM_TRACER_SERVER_H
#define NBM_TRACER_SERVER_H

#include "Network.h"
#include "Threadpool.h"
#include "Tasks/RenderPixel.h"
#include "Tasks/BatchedRender.h"

//#define OBJ_FILE "/home/agmui/cs/networks/NBM_Tracer/lib/muian_raytracer/resources/part5/test2.obj"
//#define OBJ_FILE "/home/agmui/cs/networks/NBM_Tracer/lib/muian_raytracer/resources/part4/test2.obj"

#define OBJ_FILE "/home/agmui/cs/networks/NBM_Tracer/lib/muian_raytracer/resources/scenes/cornell_box.obj"
#define MTL_FILE "/home/agmui/cs/networks/NBM_Tracer/lib/muian_raytracer/resources/scenes/cornell_box.mtl"
#define CLIENT_MTL_FILE "/tmp/cornell_box.mtl"

class Server
{

public:
    Server(Network &network, Threadpool &threadpool) : network(network), threadpool(threadpool) {};

    void start(char *port);
    void generateTasks();
private:
    Network &network;
    Threadpool &threadpool;
};

#endif // NBM_TRACER_SERVER_H
