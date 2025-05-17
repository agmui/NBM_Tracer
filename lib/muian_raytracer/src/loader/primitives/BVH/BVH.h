//
// Created by agmui on 1/27/25.
//

#ifndef TRACER_BVH_H
#define TRACER_BVH_H

#include <memory>
#include <vector>

#include "BVHnode.h"
#include "splitters/Splitter.h"
#include "src/loader/Camera.h"
#include "src/loader/Scene.h"
#include "src/loader/macros.h"

class Scene;
class Splitter;

class BVH {
 public:
  BVH(Splitter& splitter, const std::shared_ptr<Scene>& scene)
      : splitter(splitter), scene(scene) {}

  BVHnode helper(std::vector<std::shared_ptr<Primitive>>& primList, AABB& aabb,
                 Splitter& splitter, int lower, int upper, int depth);
  void buildTree(const Camera& camera, const std::shared_ptr<Scene>& scene);
  inline hitpoint hitRecurse(BVHnode& node, const Ray& r, float cutoffBound);
  inline hitpoint hitIterative(const Ray& r, float cutoffBound);
  hitpoint hit(const Ray& r, float cutoffBound=MAX_RAY_DIST);
  void printDotfile();
  int numNodes() { return nodeArr.size(); }

 private:
  int rootIndex = -1; // TODO: just return list len
  std::vector<BVHnode> nodeArr;
  Splitter& splitter;
  const std::shared_ptr<Scene>& scene;  //TODO: just store ref of prim list
};

#endif  //TRACER_BVH_H
