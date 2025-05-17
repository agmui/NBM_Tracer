//
// Created by agmui on 2/9/25.
//

#ifndef TRACER_SAH_H
#define TRACER_SAH_H

#include "Splitter.h"
#include "src/loader/primitives/BVH/BVHnode.h"

class SAH : public Splitter {
  int split(std::vector<std::shared_ptr<Primitive>>& primList, AABB& aabb,
            int lower, int upper) override;
};

#endif  //TRACER_SAH_H
