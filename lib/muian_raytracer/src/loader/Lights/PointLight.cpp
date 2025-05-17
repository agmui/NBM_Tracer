//
// Created by agmui on 12/16/24.
//

#include "PointLight.h"

PointLight::PointLight(const Vector3& pos, const std::shared_ptr<Material>& material)
    : pos(pos), material(material) {}
const Vector3& PointLight::getPos() const {
  return pos;
}
const std::shared_ptr<Material>& PointLight::getMaterial() const {
  return material;
}
