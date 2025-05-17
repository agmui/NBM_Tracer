//
// Created by agmui on 12/4/24.
//

#include "Camera.h"
void Camera::generateRays() {

  rays.reserve(width * height);

  const Vector3& e = pos;  //getPos();
  //  double s, c;
  //  sincos(fov / 2, &s, &c);
  //  auto d = (float) (((double)width/2) * (s/c));
  //  float d = (float)width / (tan(fov / 2) * 2);
  auto d = (float)((width / 2.0L) * tan(M_PI_2 - fov / 2));
  float l = -(float)width / 2;
  float r = -l;
  float b = -(float)height / 2;
  float t = -b;
  /*
   * If y increases in this for loop we are forced to generate the rays
   * starting at the bottom because the ray's direction is directly linked to
   * y in "space". To fix this we just loop y and decrement so in space we are
   * moving down.
   */
  for (int y = height - 1; y >= 0; --y) {
    for (int x = 0; x < width; ++x) {
      float u_pos = (float)x + .5f + l;
      float v_pos = (float)y + .5f + b;
      Vector3 se =
          u_pos * u + v_pos * v - d * w;  // applies cam's dir transform
      rays.emplace_back(e, se.normalize());
    }
  }
}
void Camera::move(const Vector3& translate, const Vector3& lookAtPoint) {
  pos += translate;

  gaze = lookAtPoint-pos;
  gaze.normalize();
  w = -this->gaze;
  u = this->upNorm.cross(w).normalize();
  v = w.cross(u).normalize();

  rays.clear();
  generateRays();
}
