//
// Created by agmui on 12/5/24.
//
#include <vector>

#include "Loader.h"
#include "macros.h"
#include "primitives/shapes/Triangle.h"

std::shared_ptr<Scene> Loader::load(char* filename) const {

  //  objLoader objData;
  //  objData.load(filename);
  auto scene =
      std::make_shared<Scene>(objData.sphereCount + objData.faceCount,
                              objData.lightPointCount + objData.lightDiscCount +
                                  objData.lightQuadCount);

  std::vector<std::shared_ptr<Material>> materials;
  materials.reserve(objData.materialCount);
  //TODO: make this the pink&black texture
  std::shared_ptr<Material> emptyMaterial = std::make_shared<Material>(
      "no mat", "no mat", Vector3(1, 1, 1), Vector3(1, 1, 1), Vector3(1, 1, 1),
      1, 1, 1, 1, 1, 1);
  for (int i = 0; i < objData.materialCount; ++i) {
    materials.push_back(OBJ2MAT(objData.materialList[i]));//TODO: use emplace back
  }

  for (int i = 0; i < objData.lightPointCount; ++i) {
    obj_light_point* lp = objData.lightPointList[i];
    auto light = std::make_shared<PointLight>(
        OBJ2GENVEC(objData.vertexList[lp->pos_index]),
        OBJ2MAT(objData.materialList[lp->material_index]));
    scene->addLight(light);
  }

  for (int sphereIndex = 0; sphereIndex < objData.sphereCount; ++sphereIndex) {

    //        typedef struct obj_sphere
    //        {
    //            int pos_index;
    //            int up_normal_index;
    //            int equator_normal_index;
    //            int texture_index[MAX_VERTEX_COUNT];
    //            int material_index;
    //        } obj_sphere;

    obj_sphere* s = objData.sphereList[sphereIndex];
    assert(s->up_normal_index != -1);
    assert(s->up_normal_index < objData.normalCount);
    assert(s->pos_index != -1);
    assert(s->pos_index < objData.vertexCount);
    assert(s->material_index < objData.materialCount);

    auto material = (s->material_index == -1) ? emptyMaterial
                                              : materials[s->material_index];
    auto sphere = std::make_shared<Sphere>(
        OBJ2GENVEC(objData.vertexList[s->pos_index]),
        OBJ2GENVEC(objData.normalList[s->up_normal_index]),
        OBJ2GENVEC(objData.normalList[s->equator_normal_index]), material);
    scene->addPrimitive(sphere);
  }

  for (int faceIndex = 0; faceIndex < objData.faceCount; ++faceIndex) {

    //        typedef struct obj_face
    //        {
    //            int vertex_index[MAX_VERTEX_COUNT];
    //            int normal_index[MAX_VERTEX_COUNT];
    //            int texture_index[MAX_VERTEX_COUNT];
    //            int vertex_count;
    //            int material_index;
    //        } obj_face;

    obj_face* f = objData.faceList[faceIndex];
    int numVertex = f->vertex_count;
    assert(numVertex == 3); //TODO: implement faces with any number of verts
    std::vector<Vector3> vertex;
    vertex.reserve(numVertex);
    assert(numVertex < objData.vertexCount);
    for (int i = 0; i < numVertex; ++i) {
      int v = f->vertex_index[i];
      if (v < 0)
        break;
      //      vertex.emplace_back(OBJ2GENVEC(objData.vertexList[v]));
      vertex.emplace_back(objData.vertexList[v]->e[0],
                          objData.vertexList[v]->e[1],
                          objData.vertexList[v]->e[2]);
    }

    std::vector<Vector3> normals;
    normals.reserve(numVertex);
    for (int i = 0; i < numVertex; ++i) {
      assert(i < objData.normalCount);
      int v = f->normal_index[i];
      if (v < 0)
        break;
      normals.emplace_back(objData.normalList[v]->e[0],
                           objData.normalList[v]->e[1],
                           objData.normalList[v]->e[2]);
    }
    auto material = (f->material_index == -1) ? emptyMaterial
                                              : materials[f->material_index];
    auto triangle = std::make_shared<Triangle>(vertex, normals, material);
    scene->addPrimitive(triangle);
  }
  //  delete_obj_data(objData);
  //  delete(&objData);
  return scene;
}
Camera Loader::loadCamera() const {
  if (objData.camera == nullptr)
    throw std::runtime_error("camera was not found in OBJ file");
  return Camera{
      OBJ2GENVEC(objData.vertexList[objData.camera->camera_pos_index]),
      OBJ2GENVEC(objData.vertexList[objData.camera->camera_look_point_index]),
      OBJ2GENVEC(objData.normalList[objData.camera->camera_up_norm_index]), RES,
      RES};
}
