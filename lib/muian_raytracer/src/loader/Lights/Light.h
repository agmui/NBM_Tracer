//
// Created by agmui on 12/14/24.
//

#ifndef TRACER_LIGHT_H
#define TRACER_LIGHT_H

#include <memory>

#include "GenVector.h"

#include "src/loader/Material.h"

/*
 * Light interface
 */
class Light {
 public:
  virtual const Vector3& getPos() const = 0;
  virtual const std::shared_ptr<Material>& getMaterial() const = 0;

};

#endif  //TRACER_LIGHT_H
