//
// Created by agmui on 12/5/24.
//

#include "Sphere.h"

#include <memory>
#include <utility>

#include "src/loader/macros.h"
#include "src/loader/primitives/hitpoint.h"

Sphere::Sphere(const Vector3& pos, const Vector3& upNorm,
               const Vector3& equatorNorm, std::shared_ptr<Material> material)
    : pos(pos),
      upNorm(upNorm),
      equatorNorm(equatorNorm),
      radius(equatorNorm.length()),
      material(std::move(material)) {}

hitpoint Sphere::hit(const Ray& ray, float cutoffBound)  {
  const Vector3& d = ray.getDirection();
  const Vector3& e = ray.getOrigin();  //TODO: sub out all e-c
  Vector3 c = this->pos;
  float r = this->radius;

  float discriminate = (d.dot(e - c)) * (d.dot(e - c)) -
                       (d.dot(d)) * ((e - c).dot(e - c) - r * r);
  //TODO: figure out material instead of nullptr
  hitpoint nothing = {nullptr, VEC_0, -1};
  if (discriminate < 0) {
    return nothing;
  } else if (discriminate == 0) {
    float t = -d.dot(e - c) / (d.dot(d));
    return {material, ((e + t * d) - c).normalize(), t};
  } else {
    float t = (-d.dot(e - c) + sqrt(discriminate)) / (d.dot(d));
    float t2 = (-d.dot(e - c) - sqrt(discriminate)) / (d.dot(d));
    if (t > cutoffBound && t2 > cutoffBound) {
      return nothing;
    } else {
      float smaller_t = (t < t2) ? t : t2;
      if (t <= VALID_HITPOINT)
        return nothing;
      return {material, ((e + smaller_t * d) - c).normalize(), smaller_t};
    }
  }
}
