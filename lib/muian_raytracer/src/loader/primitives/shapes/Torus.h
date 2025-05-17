//
// Created by agmui on 12/9/24.
//

#ifndef TRACER_TORUS_H
#define TRACER_TORUS_H

#include <algorithm>

#include <GenVector.h>
#include <algebra.h>

#include "src/loader/Material.h"
#include "src/loader/macros.h"
#include "src/loader/primitives/Primitive.h"
#include "src/loader/primitives/Ray.h"

class Torus : public Primitive {
 public:
  Torus(const Vector3& pos, float r, float a,
        std::shared_ptr<Material> material, float phi, float theta, float psi);

  hitpoint hit(const Ray& ray, float cutoffBound) override;

  float turnX(float rad) override {
    phi = (float)fmod(phi + rad, 2 * M_PI);
    return phi;
  }
  float turnY(float rad) override {
    //    theta += rad;
    theta = (float)fmod(theta + rad, 2 * M_PI);
    return theta;
  }
  float turnZ(float rad) override {
    //    psi += rad;
    psi = (float)fmod(psi + rad, 2 * M_PI);
    return psi;
  }

  void getMin(float outputPos[3]) override {};
  void getMax(float outputPos[3]) override {};

  Vector3 getCentroid() override { return pos; };

 private:
  Vector3 pos;
  float R;
  float a;
  std::shared_ptr<Material> material;
  float phi;
  float theta;
  float psi;
};

#endif  //TRACER_TORUS_H
