//
// Created by agmui on 1/27/25.
//

#ifndef TRACER_BVHNODE_H
#define TRACER_BVHNODE_H

#include <utility>

#include "src/loader/primitives/Primitive.h"
#include "src/loader/primitives/shapes/AABB.h"

class BVHnode  {
 public:
  //TODO: remove
  BVHnode(): nodeL(0), aabb(), isLeaf(false), isList(false){};
  BVHnode(uint nodeL,
          AABB& aabb,
          bool isLeaf,
          bool isList = false
          )
      : nodeL(nodeL),
        aabb(aabb),
        isLeaf(isLeaf),
        isList(isList)
  {}

  inline hitpoint hit(const Ray& ray, float cutoffBound) {
    return aabb.hit(ray, cutoffBound);
  }
  bool is_leaf() const {return isLeaf; }
  bool _isList() const {return isList; }
  uint getNodeL() const{ return nodeL; }
  uint getNodeR() const{
    return nodeL+1;
//    return nodeR;
  }

//  float turnX(float rad) override { return -1.0f; };
//  float turnY(float rad) override { return -1.0f; };
//  float turnZ(float rad) override { return -1.0f; };

  void getMin(float outputPos[3])  { aabb.getMin(outputPos); };
  void getMax(float outputPos[3])  { aabb.getMax(outputPos); };

  Vector3 getCentroid() { return aabb.getCentroid(); };

 private:
  AABB aabb;
  uint nodeL;
  bool isLeaf;
  bool isList;
};

#endif  //TRACER_BVHNODE_H
