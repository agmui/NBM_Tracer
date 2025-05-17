//
// Created by agmui on 12/5/24.
//

#include "Scene.h"
Scene::Scene(int primitiveCount, int lightCount) {
  primitiveList.reserve(primitiveCount);
  lightList.reserve(lightCount);
}
std::vector<std::shared_ptr<Primitive>>& Scene::getPrimitiveList() {
  return primitiveList;
}
const std::vector<std::shared_ptr<Light>>& Scene::getLightList() const {
  return lightList;
}

//TODO: have it return true of false instead of hp -1
void Scene::trace(hitpoint& closest_hp, const Ray& r, float cutoffBound) {
  //TODO: if hp_ret gets update use that instead of cutoffBound
  for (const auto& prim : primitiveList) {
    const hitpoint& hp = prim->hit(r, cutoffBound);
    if (hp.t != -1) {
      // update cutoff
      //  note: does not need to check for min
      //  between cutoffBound and hp.t because
      //  min is already check in hit
      cutoffBound = hp.t;
      closest_hp = hp;
    }
  }
}

void Scene::trace(hitpoint& closest_hp, const Ray& r, float cutoffBound, BVH& bvh) {
  const hitpoint& hp = bvh.hit(r, cutoffBound);
  if(hp.t != -1){
    closest_hp = hp;
  }
}

Vector3 Scene::shade(const hitpoint& hp, const Ray& r, BVH& bvh) {
  if (lightList.empty())  //TODO: add a light into the scene
                          //    return WHITE;
    return hp.normal;

  const Vector3& point = r.getOrigin() + hp.t * r.getDirection() +
                         hp.normal * 0.0001f;  //TODO: black magic const add

  Vector3 I;
  for (const auto& currLight : lightList) {

    // == ambient ==
    const Vector3& k_a = hp.material->getAmb();
    const Vector3& L_a = currLight->getMaterial()->getAmb();
    Vector3 I_a = k_a * L_a;
    I += I_a;

    // === shadows ===
    Vector3 lightDistVec =
        currLight->getPos() - point;  // starts at hp and goes to light

    if (hp.normal.dot(lightDistVec) > 0) {  // hit nothing
      Vector3 l_not_norm = lightDistVec;    //TODO: make norm const
      Vector3& l = l_not_norm.normalize();
      Ray new_r = Ray(point, l);

      hitpoint hp_shadow;
      trace(hp_shadow, new_r, lightDistVec.length(), bvh); // TODO: add cutoff lightDistVec.length()
      if (hp_shadow.t == -1) {

        // === defuse ===
        const Vector3& k_d = hp.material->getDiff();
        const Vector3& L_d = currLight->getMaterial()->getDiff();
        const Vector3& I_d = (k_d * L_d * l.dot(hp.normal));

        // === specular reflectance ===
        const Vector3& k_s = hp.material->getSpec();
        const Vector3& L_s = currLight->getMaterial()->getSpec();
        const Vector3& l_r =
            l.reflect(hp.normal);  //TODO: make reflect const so l can be ref
        const Vector3& v = r.getDirection();
        constexpr int a =
            2;  //TODO: use the shiny var in the material (add NS to .mtl)
        //    Vector3 I_s = k_s * L_s * (float)pow(l_r.dot(v), a);
        Vector3 I_s = L_s * (float)pow(l_r.dot(v), a);

        I += I_d + I_s;
      }
#ifdef OBJ_DEBUG
      if (draw) {
        draw = false;
        if (hp_shadow.t == -1)  // reflected ray
          ObjViewer::get().addRay(new_r.getOrigin(), new_r.getDirection(),
                                  lightDistVec.length());
        else
          ObjViewer::get().addRay(new_r.getOrigin(), r.getDirection(),
                                  hp_shadow.t);
      }
#endif
    }
  }
#if MAX_BOUNCES != 0
  // === reflections ===
  //TODO: move numBounces as arg for shade
  if (r.getNumBounces() < MAX_BOUNCES && hp.material->getReflect() != 0) {
    Vector3 dir = r.getDirection();
    //TODO: maybe reuse previous ray instead of make new one
    Ray reflectedRay =
        Ray(point, dir.reflect(hp.normal), r.getNumBounces() + 1);
    hitpoint hp_reflected;
    trace(hp_reflected, reflectedRay);
#ifdef OBJ_DEBUG
    ObjViewer::get().addRay(reflectedRay.getOrigin(),
                            reflectedRay.getDirection(), hp_reflected.t);
#endif
    if (hp_reflected.t != -1) {
      const Vector3& I_reflected = shade(hp_reflected, reflectedRay, bvh);
      I = (float)(1.0f - hp.material->getReflect()) * I +
          (float)hp.material->getReflect() * I_reflected;
    }
  }
#endif

#ifdef TRANSPARENCY
  // == transparency ==
  if (hp.material->getTrans() != 0) {
    Ray trans_ray = Ray(point+r.getDirection()*.1, r.getDirection());
    hitpoint hp_trans;
    trace(hp_trans, trans_ray);
    if (hp_trans.t != -1) {
      const Vector3& I_trans = shade(hp_trans, trans_ray);
      I = (float)(1.0f - hp.material->getTrans()) * I +
          (float)hp.material->getTrans() * I_trans;
    }
  }
#endif

  return I;
}
