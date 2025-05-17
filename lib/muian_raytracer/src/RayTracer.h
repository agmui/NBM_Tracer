//
// Created by agmui on 12/10/24.
//

#ifndef TRACER_RAYTRACER_H
#define TRACER_RAYTRACER_H

// For the test scenes, resolution of 100x100, and fov 90 degree, my
// generator creates the test images. My ray dirs are normalized.
#ifdef OBJ_DEBUG
#define RES 100
#else
#define RES 100
#endif

#define NUM_THREADS 12 // 1
#define _USE_MATH_DEFINES  //This enables math constants in Windows
#include <unistd.h>
#include <chrono>
#include <cmath>  //Math functions and some constants
#include <memory>
#include <thread>

#include <Buffer.h>
#include <GenVector.h>
#include <simplePNG.h>
#include <objLoader.hpp>

#include "loader/Camera.h"
#include "loader/Loader.h"
#include "loader/Scene.h"
#include "loader/macros.h"
#include "loader/primitives/Ray.h"
#include "loader/primitives/shapes/Sphere.h"
#include "loader/primitives/shapes/Torus.h"
#include "loader/primitives/shapes/Triangle.h"
#include "loader/primitives/shapes/AABB.h"
#include "loader/primitives/BVH/BVH.h"
#include "loader/primitives/BVH/splitters/Splitter.h"
#include "loader/primitives/BVH/splitters/Median.h"
#include "loader/primitives/BVH/splitters/SAH.h"
#include "loader/primitives/BVH/splitters/ObjectSplitter.h"

class RayTracer {
 public:
  RayTracer() = default;
  void printASCII(Buffer<Color> buffer, const int print_height = 20,
                  const int print_width = 40);
//  void traceOneRay(const Camera& camera, int x, int y, BVH& bvh, const std::shared_ptr<Scene>& scene, Buffer<Vector3>& intensityBuffer, float& maxIntensity);
  void render(Buffer<Color>& buffer, const Camera& camera, const std::shared_ptr<Scene>& scene, BVH& bvh);
  void run(char* filename, char* outputPNG, bool ascii = false);
};

#endif  //TRACER_RAYTRACER_H
