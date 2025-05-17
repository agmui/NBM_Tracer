//
// Created by agmui on 12/14/24.
//

#ifndef TRACER_MATERIAL_H
#define TRACER_MATERIAL_H

#include <GenVector.h>
#include <obj_parser.h>

class Material {
 public:
  Material(const char name[MATERIAL_NAME_SIZE],
           const char textureFilename[OBJ_FILENAME_LENGTH], const Vector3& amb,
           const Vector3& diff, const Vector3& spec, double reflect,
           double refract, double trans, double shiny, double glossy,
           double refractIndex);
  const char* getName() const;
  const char* getTextureFilename() const;
  const Vector3& getAmb() const;
  const Vector3& getDiff() const;
  const Vector3& getSpec() const;
  double getReflect() const;
  double getRefract() const;
  double getTrans() const;
  double getShiny() const;
  double getGlossy() const;
  double getRefractIndex() const;

 private:
  char name[MATERIAL_NAME_SIZE]{};
  char texture_filename[OBJ_FILENAME_LENGTH]{};
  Vector3 amb;
  Vector3 diff;
  Vector3 spec;
  double reflect{0};
  double refract{0};
  double trans{0};
  double shiny{0};
  double glossy{0};
  double refract_index{0};

  //  typedef struct obj_material
  //  {
  //    char name[MATERIAL_NAME_SIZE];
  //    char texture_filename[OBJ_FILENAME_LENGTH];
  //    double amb[3];
  //    double diff[3];
  //    double spec[3];
  //    double reflect;
  //    double refract;
  //    double trans;
  //    double shiny;
  //    double glossy;
  //    double refract_index;
  //  } obj_material;
};

#endif  //TRACER_MATERIAL_H
