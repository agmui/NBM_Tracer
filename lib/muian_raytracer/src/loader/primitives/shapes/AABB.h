//
// Created by agmui on 1/21/25.
//

#ifndef TRACER_AABB_H
#define TRACER_AABB_H

#include <utility>

#include "src/loader/macros.h"
#include "src/loader/primitives/Primitive.h"
#include "src/loader/primitives/hitpoint.h"

#define NUMDIM 3
#define RIGHT 0
#define LEFT 1
#define MIDDLE 2
class AABB  {
 public:
  //TODO: remove
  AABB(): bbMin{0,0,0}, bbMax{0,0,0}{}
  AABB(float bbMin_x, float bbMin_y, float bbMin_z, float bbMax_x,
       float bbMax_y, float bbMax_z
       //       , std::shared_ptr<Material> material
       )
      : bbMin{bbMin_x, bbMin_y, bbMin_z},
        bbMax{bbMax_x, bbMax_y, bbMax_z}
  //        material(std::move(material))
  {}

inline bool IntersectAABB(const Ray& ray, float cutoffBound) {
  float tx1 = (bbMin[0] - ray.getOrigin()[0]) / ray.getDirection()[0],
        tx2 = (bbMax[0] - ray.getOrigin()[0]) / ray.getDirection()[0];
  float tmin = std::min(tx1, tx2), tmax = std::max(tx1, tx2);
  float ty1 = (bbMin[1] - ray.getOrigin()[1]) / ray.getDirection()[1],
        ty2 = (bbMax[1] - ray.getOrigin()[1]) / ray.getDirection()[1];
  tmin = std::max(tmin, std::min(ty1, ty2)), tmax = std::min(tmax, std::max(ty1, ty2));
  float tz1 = (bbMin[2] - ray.getOrigin()[2]) / ray.getDirection()[2],
        tz2 = (bbMax[2] - ray.getOrigin()[2]) / ray.getDirection()[2];
  tmin = std::max(tmin, std::min(tz1, tz2)), tmax = std::min(tmax, std::max(tz1, tz2));
  return tmax >= tmin && tmin < cutoffBound && tmax > 0;
}

  hitpoint HitBoundingBox(const Ray& r, float cutoffBound) {
    float coord[3];
    const Vector3& origin = r.getOrigin();
    const Vector3& dir = r.getDirection();

    char inside = true;
    char quadrant[NUMDIM];
    int i;
    int whichPlane;
    float maxT[NUMDIM];
    float candidatePlane[NUMDIM];

    /* Find candidate planes; this loop can be avoided if
      rays cast all from the eye(assume perspective view) */
    for (i = 0; i < NUMDIM; i++)
      if (origin[i] < bbMin[i]) {
        quadrant[i] = LEFT;
        candidatePlane[i] = bbMin[i];
        inside = false;
      } else if (origin[i] > bbMax[i]) {
        quadrant[i] = RIGHT;
        candidatePlane[i] = bbMax[i];
        inside = true;
      } else {
        quadrant[i] = MIDDLE;
      }

    /* Ray origin inside bounding box */
    if (inside) {
      //      coord = origin;
      //      return (true);
      return {nullptr, {0, 0, 0}, 1};
    }

    /* Calculate T distances to candidate planes */
    for (i = 0; i < NUMDIM; i++)
      if (quadrant[i] != MIDDLE && dir[i] != 0.)
        maxT[i] = (candidatePlane[i] - origin[i]) / dir[i];
      else
        maxT[i] = -1.;

    /* Get largest of the maxT's for final choice of intersection */
    whichPlane = 0;
    for (i = 1; i < NUMDIM; i++)
      if (maxT[whichPlane] < maxT[i])
        whichPlane = i;

    /* Check final candidate actually inside box */
    if (maxT[whichPlane] < 0. && maxT[whichPlane] < cutoffBound)
      //      return (false);
      return {nullptr, nullptr, -1};
    for (i = 0; i < NUMDIM; i++)
      if (whichPlane != i) {
        coord[i] = origin[i] + maxT[whichPlane] * dir[i];
        if (coord[i] < bbMin[i] || coord[i] > bbMax[i])
          //          return (false);
          return {nullptr, nullptr, -1};
      } else {
        coord[i] = candidatePlane[i];
      }
    //    return (true); /* ray hits box */
    return {nullptr, {0, 0, 0}, maxT[whichPlane]};
  }


  inline hitpoint hit(const Ray& ray, float cutoffBound) {
    float b[2] = {-1.0,1.0};
    return {nullptr, VEC_0, b[IntersectAABB(ray, cutoffBound)]};

    //    return HitBoundingBox(ray, cutoffBound);

    //we want to find the farthest entrance and closest exit to the box
    //if the exit is closer than the entrance, there is no hit
    const size_t vecDim = 3;
    float entrance = 0.0f;
    float exit = cutoffBound;
    Vector3 normal = Vector3(0, 0, 0);

    for (int i = 0; i < vecDim; i++) {
      float slabA = bbMin[i];
      float slabB = bbMax[i];
      float invDir = 1.0f / ray.getDirection()[i];
      float origin = ray.getOrigin()[i];

      float closestHit = (slabA - origin) * invDir;
      float farthestHit = (slabB - origin) * invDir;

      if (farthestHit < closestHit)
        std::swap(closestHit, farthestHit);

      bool tooClose = farthestHit < entrance;
      bool tooFar = closestHit > exit;

      if (tooClose || tooFar)
        return {nullptr, VEC_0, -1};

      bool foundNewEntrance = closestHit > entrance;
      entrance = foundNewEntrance ? closestHit : entrance;

      bool foundNewExit = farthestHit < exit;
      exit = foundNewExit ? farthestHit : exit;

      //      if (foundNewEntrance) {
      //        normal = Vector3(0, 0, 0);
      //        normal[i] = ray.getDirection()[i] * -1;
      //        normal.normalize();
      //      }
    }
    return {//      material,
            nullptr, normal, entrance};
  }

//  float turnX(float rad) override { return 0; }
//  float turnY(float rad) override { return 0; }
//  float turnZ(float rad) override { return 0; }
//
  void getMin(const float outputPos[3]) { outputPos = bbMin; }
  void getMax(const float outputPos[3]) { outputPos = bbMax; }

  Vector3 getCentroid() {
    return {(bbMin[0] + bbMax[0]) / 2.f, (bbMin[1] + bbMax[1]) / 2.f,
            (bbMin[2] + bbMax[2]) / 2.f};
  }
  const float* getBbMin() const { return bbMin; }
  const float* getBbMax() const { return bbMax; }

 private:
  float bbMin[3];
  float bbMax[3];
  //  std::shared_ptr<Material> material;
};

#endif  //TRACER_AABB_H
