//
// Created by agmui on 12/14/24.
//

#ifndef TRACER_HITPOINT_H
#define TRACER_HITPOINT_H

#include <memory>

#include <GenVector.h>

#include "src/loader/Material.h"

/*
 * if t is -1 then the hitpoint is invalid
 */
struct hitpoint {
  std::shared_ptr<Material> material;
  Vector3 normal;
  float t = -1; //TODO: maybe return point instead of t
};

#endif  //TRACER_HITPOINT_H
