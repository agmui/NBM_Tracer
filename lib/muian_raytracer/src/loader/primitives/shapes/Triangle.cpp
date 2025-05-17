//
// Created by agmui on 12/9/24.
//

#include "Triangle.h"

#include <utility>

Triangle::Triangle(const std::vector<Vector3>& vertex,
                   const std::vector<Vector3>& normals,
                   std::shared_ptr<Material> material)
    : vertex{vertex[0], vertex[1], vertex[2]},
//      normals{normals[0], normals[1], normals[2]},
      normal((vertex[1] - vertex[0]).cross(vertex[2] - vertex[1]).normalize()),
      material(std::move(material)),
      ba(vertex[1] - vertex[0]),
      cb(vertex[2] - vertex[1]),
      ac(vertex[0] - vertex[2])
//      ca(vertex[2] - vertex[0])
{}

inline hitpoint Triangle::hit(const Ray& ray, float cutoffBound) {
//  hitpoint hp;
//  if(fasterHit(ray, hp)){
//    return hp;
//  }
//  return { nullptr,VEC_0,-1};

//  /*
  const Vector3& e = ray.getOrigin();
  const Vector3& d = ray.getDirection();
  const Vector3& a = vertex[0];
  const Vector3& b = vertex[1];
  const Vector3& c = vertex[2];
  const Vector3& n = normal;

  // back face culling may also check if ray is parallel at the same time?
  float face_alignment = d.dot(n);
  if (face_alignment >= 0)
    return {nullptr, VEC_0, -1};  //TODO: figure out material instead of nullptr

  float t = ((a - e).dot(n)) / face_alignment;
  // TODO: profile to see if you check sign before div
  //  and exit early if its faster

  //TODO: check if ray is parallel
  //  const Vector3& ray_cross_ba = d.cross(ba);
  //  float det = ba.dot(ray_cross_ba);
  //  if(det > -EPSILON && det < EPSILON)
  //    return nothing;
  if (t > cutoffBound || t <= VALID_HITPOINT)  //-0.0f)
    return {nullptr, VEC_0, -1};
  const Vector3& x = e + t * d;
  bool inbounds = (ba.cross(x - a).dot(n) > 0 && cb.cross(x - b).dot(n) > 0 &&
                   ac.cross(x - c).dot(n) > 0);
  if (inbounds) {
    return {material, normal, t};
  } else {
    return {nullptr, VEC_0, -1};  //TODO: make it a macro for invalid hp
  }
//   */
}
