//
// Created by agmui on 12/9/24.
//

#include "Torus.h"

#include <utility>

Torus::Torus(const Vector3& pos, float r, float a,
             std::shared_ptr<Material> material, float phi = 0, float theta = 0,
             float psi = 0)
    : pos(pos),
      R(r),
      a(a),
      material(std::move(material)),
      phi(phi),
      theta(theta),
      psi(psi) {}

hitpoint Torus::hit(const Ray& ray, float cutoffBound) {
  const Vector3 d = rotZ(rotY(rotX(ray.getDirection(), phi), theta), psi);
  const Vector3 c = pos;
  const Vector3 e = rotZ(rotY(rotX(ray.getOrigin() - c, phi), theta), psi);

  //  equation from http://cosinekitty.com/raytrace/chapter13_torus.html
  const double T = 4 * R * R;
  const double G = T * (d[0] * d[0] + d[1] * d[1]);
  const double H = 2 * T * (e[0] * d[0] + e[1] * d[1]);
  const double I = T * (e[0] * e[0] + e[1] * e[1]);
  const double J =
      d[0] * d[0] + d[1] * d[1] + d[2] * d[2];  //d.squaredLength();
  const double K = 2 * (e[0] * d[0] + e[1] * d[1] + e[2] * d[2]);  //2*e.dot(d);
  const double L = e[0] * e[0] + e[1] * e[1] + e[2] * e[2] + R * R -
                   a * a;  //e.squaredLength()+(R*R-a*a);

  double roots[4];

  const int numRealRoots = Algebra::SolveQuarticEquation(
      J * J,                    // coefficient of t^4
      2.0 * J * K,              // coefficient of t^3
      2.0 * J * L + K * K - G,  // coefficient of t^2
      2.0 * K * L - H,          // coefficient of t^1 = u
      L * L - I,                // coefficient of t^0 = constant term
      roots                     // receives 0..4 real solutions
  );

  //TODO: figure out material instead of nullptr
  hitpoint nothing = {nullptr, VEC_0, -1};
  if (numRealRoots <= 0)
    return nothing;
  double smallestRoot = *std::min_element(roots, roots + numRealRoots);
  if (smallestRoot < cutoffBound) {
    auto t = (float)smallestRoot;
    if (t <= VALID_HITPOINT)
      return nothing;
    Vector3 p = ray.getOrigin() + t * ray.getDirection();
    float alpha = a / sqrt(p[0] * p[0] + p[1] * p[1]);
    Vector3 normal =
        Vector3((1 - alpha) * p[0], (1 - alpha) * p[1], p[2]).normalize();
    return {material, normal, t};
  } else {
    return nothing;
  }
}
