//
// Created by agmui on 12/16/24.
//

#ifndef TRACER_POINTLIGHT_H
#define TRACER_POINTLIGHT_H

#include "Light.h"

class PointLight : public Light {
 public:
  PointLight(const Vector3& pos, const std::shared_ptr<Material>& material);
  const Vector3& getPos() const override;
  const std::shared_ptr<Material>& getMaterial() const override;

 private:
  Vector3 pos;
  std::shared_ptr<Material> material;
};

#endif  //TRACER_POINTLIGHT_H
