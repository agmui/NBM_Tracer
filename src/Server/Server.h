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

//#define OBJ_FILE "/home/agmui/cs/networks/NBM_Tracer/lib/muian_raytracer/resources/scenes/cornell_box.obj"
//#define MTL_FILE "/home/agmui/cs/networks/NBM_Tracer/lib/muian_raytracer/resources/scenes/cornell_box.mtl"
//#define CLIENT_MTL_FILE "/tmp/cornell_box.mtl"

//#define OBJ_FILE "/home/agmui/cs/networks/NBM_Tracer/lib/muian_raytracer/resources/scenes/blue_sphere.obj"
//#define MTL_FILE "/home/agmui/cs/networks/NBM_Tracer/lib/muian_raytracer/resources/scenes/blue_sphere.mtl"
//#define CLIENT_MTL_FILE "/tmp/blue_sphere.mtl"

//#define OBJ_FILE "/home/agmui/cs/networks/NBM_Tracer/lib/muian_raytracer/resources/scenes/bigger/bunny-scene.obj"
//#define MTL_FILE "/home/agmui/cs/networks/NBM_Tracer/lib/muian_raytracer/resources/scenes/bigger/bunny-scene.mtl"
//#define CLIENT_MTL_FILE "/tmp/bunny-scene.mtl"

//#define OBJ_FILE "/home/agmui/cs/networks/NBM_Tracer/lib/muian_raytracer/resources/scenes/bigger/happy-scene.obj"
//#define MTL_FILE "/home/agmui/cs/networks/NBM_Tracer/lib/muian_raytracer/resources/scenes/bigger/happy-scene.mtl"
//#define CLIENT_MTL_FILE "/tmp/happy-scene.mtl"

//TODO: move to constructor


class Server {

public:
    Server(Network &network, Threadpool &threadpool,
           const char *OBJ_FILE,
           const char *MTL_FILE,
           const char *CLIENT_MTL_FILE,
           int msgSideLen = 100) :
            network(network),
            threadpool(threadpool),
            OBJ_FILE(OBJ_FILE),
            MTL_FILE(MTL_FILE),
            CLIENT_MTL_FILE(CLIENT_MTL_FILE),
            renderMsgDim(msgSideLen)
            {};

    void start(char *port);

    void generateTasks();

private:
    Network &network;
    Threadpool &threadpool;
    const char *OBJ_FILE;
    const char *MTL_FILE;
    const char *CLIENT_MTL_FILE;
    int renderMsgDim; // square dimensions of how many pixels to render for each task
};

#endif // NBM_TRACER_SERVER_H
