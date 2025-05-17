//
// Created by agmui on 12/5/24.
//

#ifndef TRACER_SCENE_H
#define TRACER_SCENE_H

#include <vector>
#include <memory>
#include <algorithm>
#include <cmath>

#include "Lights/Light.h"
#include "Lights/PointLight.h"
#include "primitives/Primitive.h"
#include "primitives/shapes/Sphere.h"
#include "primitives/shapes/Triangle.h"
#include "src/obj_debugger/ObjViewer.h"
#include "src/loader/primitives/BVH/BVH.h"

class BVH;

class Scene {

 public:
  explicit Scene(int primitiveCount, int lightCount);

  void trace(hitpoint& closest_hp, const Ray& r, float cutoffBound=MAX_RAY_DIST);
  void trace(hitpoint& closest_hp, const Ray& r, float cutoffBound, BVH& bvh);
  Vector3 shade(const hitpoint& hp, const Ray& r, BVH& bvh);
  void addPrimitive(const std::shared_ptr<Primitive>& primitive) { primitiveList.push_back(primitive); }
  void addLight(const std::shared_ptr<Light>& light) { lightList.push_back(light); }
  std::vector<std::shared_ptr<Primitive>>& getPrimitiveList() ;
  const std::vector<std::shared_ptr<Light>>& getLightList() const;

 private:
  std::vector<std::shared_ptr<Primitive>> primitiveList;
  //TODO: maybe store materials vec in scene?
  std::vector<std::shared_ptr<Light>> lightList;
};

#endif  //TRACER_SCENE_H
