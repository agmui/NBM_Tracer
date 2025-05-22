//
// Created by agmui on 5/16/25.
//

#include "MuianRayTracer.h"

void MuianRayTracer::init() {
  uint64_t startTime = TIMENOW();
//  SAH s = SAH();
  bvh.buildTree(camera, scene);
//  bvh.printDotfile();
  printf("build tree: %lu us\n", TIMENOW() - startTime);
  printf("num nodes in bvh: %d\n", bvh.numNodes());
  printf("bvh node msgSize: %lu\n", sizeof(BVHnode));
  printf("aabb: %lu\n", sizeof(AABB));
//  startTime = TIMENOW();
//  render(buffer, camera, scene, bvh);
//  auto rez = TIMENOW() - startTime;
//  printf("render: %lu us | %lu sec\n", rez, rez / 1000000);
//
//  //Write output buffer to file
//  simplePNG_write(outputPNG, buffer.getWidth(), buffer.getHeight(),
//                  (unsigned char*)&buffer.at(0, 0));
}

pixel MuianRayTracer::renderPixel(int x, int y) {

    const Ray& r =
            camera.getRay(x, y);  //TODO: maybe pass cam into raygen interface
    //Convert vectors to RGB colors for testing results
    Vector3 intensity = r.getDirection(); //VEC_0;

    hitpoint hp;
    //      scene->trace(hp, r);
    scene->trace(hp, r, MAX_RAY_DIST, bvh);

    if (hp.t != -1)
        intensity = scene->shade(hp, r, bvh);
//    intensityBuffer.at(x, y) = intensity;
//    maxIntensity = std::max(maxIntensity, intensity.squaredLength()); //TODO: not thread safe
    return pixel(intensity[0], intensity[1], intensity[2]);
}
