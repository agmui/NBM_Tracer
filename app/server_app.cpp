//
// Created by agmui on 5/13/25.
//

#include "TracerImplementations/TracerInterface.h"
#include "Server/Server.h"

#include "muian_raytracer/src/RayTracer.h"

int main(int argc, char **argv) {
    if (argc != 6) {
        printf("Usage: ./server <port> <RES> <.obj file> <.mtl file> </tmp/.mtl>");
        exit(1);
    }

    RES = atoi(argv[2]);//1000;
    msgSideLen = 100;

    OBJ_FILE =  argv[3];
    MTL_FILE  = argv[4];
    CLIENT_MTL_FILE  = argv[5];

//    OBJ_FILE =  "/home/agmui/cs/networks/NBM_Tracer/lib/muian_raytracer/resources/scenes/cornell_box.obj";
//    MTL_FILE  = "/home/agmui/cs/networks/NBM_Tracer/lib/muian_raytracer/resources/scenes/cornell_box.mtl";
//    CLIENT_MTL_FILE  = "/tmp/cornell_box.mtl";

//    OBJ_FILE = "/home/agmui/cs/networks/NBM_Tracer/lib/muian_raytracer/resources/scenes/bigger/happy-scene.obj";
//    MTL_FILE = "/home/agmui/cs/networks/NBM_Tracer/lib/muian_raytracer/resources/scenes/bigger/happy-scene.mtl";
//    CLIENT_MTL_FILE = "/tmp/happy-scene.mtl";


    Network network{};
    Threadpool threadpool(MAX_NUM_THREADS, network);
    Server server(network, threadpool);
    server.start(argv[1]);


    Buffer<Color> buffer = Buffer<Color>(RES, RES);
    Buffer<Vector3> intensityBuffer = Buffer<Vector3>(RES, RES);
    float maxIntensity = 0;
/*
    for (auto &i: threadpool.getResults()) {
        RenderPixelResult *result = reinterpret_cast<RenderPixelResult *>(i.get());
        Vector3 intensity{result->p.r, result->p.g, result->p.b};
        intensityBuffer.at(result->x, result->y) = intensity;
        maxIntensity = std::max(maxIntensity, intensity.squaredLength());
    }
*/

    for (auto &i: threadpool.getResults()) {
        BatchedRenderResult *result = reinterpret_cast<BatchedRenderResult *>(i.get());
        for (int y = 0; y < result->output.getHeight(); ++y) {
            for (int x = 0; x < result->output.getWidth(); ++x) {
                auto pixelResult = result->output.at(x, y);
                Vector3 intensity{pixelResult.r, pixelResult.g, pixelResult.b};
                intensityBuffer.at(result->x+x, result->y+y) = intensity;
                maxIntensity = std::max(maxIntensity, intensity.squaredLength());
            }
        }
    }
    for (int y = 0; y < RES; ++y) {
        for (int x = 0; x < RES; ++x) {
            buffer.at(x, y) = VEC2COLOR(intensityBuffer.at(x, y) / sqrt(maxIntensity));
        }
    }

    printf("===writing to png===\n");
    char *outputPNG = "output.png";
    //Write output buffer to file
    simplePNG_write(outputPNG, buffer.getWidth(), buffer.getHeight(),
                    (unsigned char *) &buffer.at(0, 0));
}