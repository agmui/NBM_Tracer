//
// Created by agmui on 12/4/24.
//

#ifndef TRACER_RAY_H
#define TRACER_RAY_H

#include <GenVector.h>

class Ray {
 private:
  Vector3 origin;
  Vector3 direction;
  int numBounces;

 public:
  Ray()
      : origin(Vector3(0, 0, 0)), direction(Vector3(0, 0, 0)), numBounces(0) {}
  Ray(const Vector3& origin, const Vector3& direction, int numBounces = 0)
      : origin(origin), direction(direction), numBounces(numBounces) {}

  const Vector3& getDirection() const { return direction; }

  const Vector3& getOrigin() const { return origin; }

  int getNumBounces() const { return numBounces; }
};

#endif  //TRACER_RAY_H
