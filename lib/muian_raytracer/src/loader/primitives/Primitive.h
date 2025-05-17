//
// Created by agmui on 12/5/24.
//

#ifndef TRACER_PRIMITIVE_H
#define TRACER_PRIMITIVE_H

#include "Ray.h"

#include "hitpoint.h"

class Primitive {

 public:
  //TODO: test with hp as arg
  inline virtual hitpoint hit(const Ray& ray, float cutoffBound) = 0;
  //TODO: remove turn for primitive
  virtual float turnX(float rad) = 0;
  virtual float turnY(float rad) = 0;
  virtual float turnZ(float rad) = 0;

  virtual void getMin(float outputPos[3]) = 0;
  virtual void getMax(float outputPos[3]) = 0;

  virtual Vector3 getCentroid() = 0;
};

#endif  //TRACER_PRIMITIVE_H
