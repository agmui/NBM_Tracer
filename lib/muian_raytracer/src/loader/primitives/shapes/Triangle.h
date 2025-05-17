//
// Created by agmui on 12/9/24.
//

#ifndef TRACER_TRIANGLE_H
#define TRACER_TRIANGLE_H

#include <memory>
#include <vector>

#include <GenVector.h>

#include "src/loader/Material.h"
#include "src/loader/macros.h"
#include "src/loader/primitives/Primitive.h"
#include "src/loader/primitives/Ray.h"

class Triangle : public Primitive {
 public:
  Triangle(const std::vector<Vector3>& vertex,
           const std::vector<Vector3>& normals,
           std::shared_ptr<Material> material);

  //    typedef struct obj_face
  //    {
  //        int vertex_index[MAX_VERTEX_COUNT];
  //        int normal_index[MAX_VERTEX_COUNT];
  //        int texture_index[MAX_VERTEX_COUNT];
  //        int vertex_count;
  //        int material_index;
  //    } obj_face;

  inline bool fasterHit(const Ray& r, hitpoint& hp) {
    float help[3];
    float *t = help;
    float *u = help+1;
    float *v = help+2;

    /* find vectors for two edges sharing vert0 */
    Vector3 edge1 = ba; //vertex[1] - vertex[0];
    Vector3 edge2 = ca; //vertex[2] - vertex[0];

    /*begin calculating determinant - also used to calculate u parameter*/
    Vector3 pvec =  r.getDirection().cross(edge2);

    /*if determinant is near zero, ray lies in plane of triangle */
    float det = edge1.dot(pvec);

    if (det < EPSILON)
      return false;

    /* calculate distance from vert0 to ray origin*/
    Vector3 tvec = r.getOrigin()- vertex[0];

    /*calculate U parameter and test bounds */
    *u = tvec.dot(pvec);
    if (*u < 0.0 || *u > det)
      return false;

    /*prepare to test V parameter */
    Vector3 qvec = tvec.cross(edge1);

    /*calculate V parameter and test bounds */
    *v = r.getDirection().dot(qvec);
    if (*v < 0.0 || *u + *v > det)
      return false;
    /* calculate t, scale parameters, ray intersects triangle */
    *t = edge2.dot(qvec);
    float inv_det = 1.0f / det;
    *t *= inv_det;
    *u *= inv_det;
    *v *= inv_det;

    hp.material = material;
    hp.normal = normal;
    hp.t = *t;
    return true;
  }

  inline hitpoint hit(const Ray& ray, float cutoffBound) override;

  float turnX(float rad) override { return 0; }
  float turnY(float rad) override { return 0; }
  float turnZ(float rad) override { return 0; }

  void getMin(float outputPos[3]) override {
    outputPos[0] = std::min(std::min(vertex[0][0], vertex[1][0]), vertex[2][0]);
    outputPos[1] = std::min(std::min(vertex[0][1], vertex[1][1]), vertex[2][1]);
    outputPos[2] = std::min(std::min(vertex[0][2], vertex[1][2]), vertex[2][2]);
  }
  void getMax(float outputPos[3]) override {
    outputPos[0] = std::max(std::max(vertex[0][0], vertex[1][0]), vertex[2][0]);
    outputPos[1] = std::max(std::max(vertex[0][1], vertex[1][1]), vertex[2][1]);
    outputPos[2] = std::max(std::max(vertex[0][2], vertex[1][2]), vertex[2][2]);
  }

  Vector3 getCentroid() override {
    return {(vertex[0][0] + vertex[1][0] + vertex[2][0]) / 3.f,
            (vertex[0][1] + vertex[1][1] + vertex[2][1]) / 3.f,
            (vertex[0][2] + vertex[1][2] + vertex[2][2]) / 3.f};
  };

 private:
  Vector3 vertex[3];
//  Vector3 normals[3];
  Vector3 normal;
  std::shared_ptr<Material> material;

  const Vector3 ba;
  const Vector3 cb;
  const Vector3 ac;
  const Vector3 ca;
};

#endif  //TRACER_TRIANGLE_H
