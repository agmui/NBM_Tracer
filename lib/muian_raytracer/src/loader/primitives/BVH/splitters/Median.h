//
// Created by agmui on 1/27/25.
//

#ifndef TRACER_MEDIAN_H
#define TRACER_MEDIAN_H

#include "Splitter.h"

class Median : public Splitter {
  int split(std::vector<std::shared_ptr<Primitive>>& primList, AABB& aabb,
            int lower, int upper) override;
};

#endif  //TRACER_MEDIAN_H
