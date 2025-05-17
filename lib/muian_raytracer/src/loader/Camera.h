//
// Created by agmui on 12/4/24.
//

#ifndef TRACER_CAMERA_H
#define TRACER_CAMERA_H

#include <vector>

#include <GenVector.h>

#include "src/loader/primitives/Ray.h"

class Camera {
 private:
  void generateRays();

 public:
  Camera(const Vector3& pos, const Vector3& lookAtPoint, const Vector3& upNorm,
         int width, int height)
      : pos(pos),
        gaze(lookAtPoint - pos),
        upNorm(upNorm),
        width(width),
        height(height) {
    this->gaze.normalize();
    this->upNorm.normalize();
    w = -this->gaze;
    u = this->upNorm.cross(w).normalize();
    v = w.cross(u).normalize();
    generateRays();
  }

  void move(const Vector3& translate, const Vector3& lookAtPoint);

  const Ray& getRay(int x, int y) const { return rays[x + y * width]; };

  const Vector3& getPos() const { return pos; }

  const Vector3& getGaze() const { return gaze; }

  const Vector3& getUpNorm() const { return upNorm; }

  const Vector3& getU() const { return u; }

  const Vector3& getV() const { return v; }

  const Vector3& getW() const { return w; }

 private:
  const float fov = M_PI_2f;
  Vector3 pos;
  Vector3 gaze;
  Vector3 upNorm;
  Vector3 u, v, w;
  int width;
  int height;
  std::vector<Ray> rays;
};

#endif  //TRACER_CAMERA_H
