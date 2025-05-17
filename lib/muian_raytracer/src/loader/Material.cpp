//
// Created by agmui on 12/14/24.
//

#include "Material.h"
Material::Material(const char name[MATERIAL_NAME_SIZE],
                   const char textureFilename[OBJ_FILENAME_LENGTH],
                   const Vector3& amb, const Vector3& diff, const Vector3& spec,
                   double reflect, double refract, double trans, double shiny,
                   double glossy, double refractIndex)
    : amb(amb),
      diff(diff),
      spec(spec),
      reflect(reflect),
      refract(refract),
      trans(trans),
      shiny(shiny),
      glossy(glossy),
      refract_index(refractIndex) {
  strncpy(this->name, name, MATERIAL_NAME_SIZE);
  strncpy(this->texture_filename, textureFilename, OBJ_FILENAME_LENGTH);
}
const char* Material::getName() const {
  return name;
}
const char* Material::getTextureFilename() const {
  return texture_filename;
}
const Vector3& Material::getAmb() const {
  return amb;
}
const Vector3& Material::getDiff() const {
  return diff;
}
const Vector3& Material::getSpec() const {
  return spec;
}
double Material::getReflect() const {
  return reflect;
}
double Material::getRefract() const {
  return refract;
}
double Material::getTrans() const {
  return trans;
}
double Material::getShiny() const {
  return shiny;
}
double Material::getGlossy() const {
  return glossy;
}
double Material::getRefractIndex() const {
  return refract_index;
}
