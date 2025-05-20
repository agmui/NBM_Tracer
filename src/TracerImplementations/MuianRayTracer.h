//
// Created by agmui on 5/16/25.
//

#ifndef NBM_TRACER_MUIANRAYTRACER_H
#define NBM_TRACER_MUIANRAYTRACER_H

#include <memory>

#include "TracerImplementations/TracerInterface.h"
#include "muian_raytracer/src/RayTracer.h"

class MuianRayTracer : public TracerInterface {
public:
    MuianRayTracer(const char* filename) : filename(const_cast<char*>(filename)), scene(loader.load(const_cast<char*>(filename))), camera(loader.loadCamera()), bvh(s,scene){

    }

    void init() override;

    pixel renderPixel(int x, int y) override;

private:
//    char* filename = "/home/agmui/cs/networks/NBM_Tracer/lib/muian_raytracer/resources/part5/test1.obj";
//    char* filename = "/home/agmui/cs/networks/NBM_Tracer/lib/muian_raytracer/resources/scenes/cornell_box.obj";
    char* filename;
    Buffer<Color> buffer= Buffer<Color>(RES, RES);
    Loader loader = Loader(filename);
    Camera camera;
    std::shared_ptr<Scene> scene;
    Median s = Median();
    BVH bvh;
};


#endif //NBM_TRACER_MUIANRAYTRACER_H
