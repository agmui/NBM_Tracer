//
// Created by agmui on 1/27/25.
//

#ifndef TRACER_SPLITTER_H
#define TRACER_SPLITTER_H

#include <vector>

#include "src/loader/Scene.h"
#include "src/loader/primitives/shapes/AABB.h"

class Splitter {
 public:
  inline virtual int split(std::vector<std::shared_ptr<Primitive>>& primList,
                           AABB& aabb, int lower, int upper) = 0;
};

#endif  //TRACER_SPLITTER_H
