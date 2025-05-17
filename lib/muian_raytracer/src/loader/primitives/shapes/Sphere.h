//
// Created by agmui on 12/5/24.
//

#ifndef TRACER_SPHERE_H
#define TRACER_SPHERE_H

#include <memory>

#include <GenVector.h>

#include "src/loader/Material.h"
#include "src/loader/primitives/Primitive.h"
#include "src/loader/primitives/Ray.h"

class Sphere : public Primitive {
 public:
  Sphere(const Vector3& pos, const Vector3& upNorm, const Vector3& equatorNorm,
         std::shared_ptr<Material> material);

  hitpoint hit(const Ray& ray, float cutoffBound) override;

  float turnX(float rad) override { return 0; }
  float turnY(float rad) override { return 0; }
  float turnZ(float rad) override { return 0; }

  void getMin(float outputPos[3]) override {
    outputPos[0] = pos[0] - radius;
    outputPos[1] = pos[1] - radius;
    outputPos[2] = pos[2] - radius;
  }
  void getMax(float outputPos[3]) override {
    outputPos[0] = pos[0] + radius;
    outputPos[1] = pos[1] + radius;
    outputPos[2] = pos[2] + radius;
  }

  Vector3 getCentroid() override { return pos; }

 private:
  Vector3 pos;
  Vector3 upNorm;
  Vector3 equatorNorm;
  float radius;
  std::shared_ptr<Material> material;

  //        typedef struct obj_sphere
  //        {
  //            int pos_index;
  //            int up_normal_index;
  //            int equator_normal_index;
  //            int texture_index[MAX_VERTEX_COUNT];
  //            int material_index;
  //        } obj_sphere;
};

#endif  //TRACER_SPHERE_H
