//
// Created by agmui on 12/22/24.
//

#ifdef OBJ_DEBUG
#include "ObjViewer.h"

bool ObjViewer::initialized = false;

/**
 * guaranteed to generate an orthogonal vector given a single vector
 */
Vector3 orthogonalVec(const Vector3& vec) {
  Vector3 v = vec;
  if (std::abs(v[0]) < std::abs(v[1]) && std::abs(v[0]) < std::abs(v[2])) {
    return {0, -v[2], v[1]};  // Use (0, z, -y) when x is smallest
  } else if (std::abs(v[1]) < std::abs(v[2])) {
    return {-v[2], 0, v[0]};  // Use (-z, 0, x) when y is smallest
  } else {
    return {v[1], -v[0], 0};  // Use (y, -x, 0) when z is smallest
  }
}

void rotateVec(Vector3& p, const Vector3& u, const Vector3& v,
               const Vector3& w) {
  float x = p[0];
  float y = p[1];
  float z = p[2];
  p[0] = x * u[0] + y * v[0] + z * w[0];
  p[1] = x * u[1] + y * v[1] + z * w[1];
  p[2] = x * u[2] + y * v[2] + z * w[2];
}

/**
 *  given a model defined in local coordinate system,
 *  rotate it to be placed in the global coordinate system
 *
 *  Note: the u v w basis vectors is how you want your model
 *  to be rotated
 */
void rotateFrame(Vector3 frame[], int num_vert, const Vector3& u,
                 const Vector3& v, const Vector3& w) {
  for (int i = 0; i < num_vert; ++i) {
    // apply transformation matrix to every point
    Vector3& p = frame[i];
    rotateVec(p, u, v, w);
  }
}

void generateLightDisk(const Vector3& normal, Vector3 frame[], int num_vert) {
  double len = normal.length();
  for (int i = 0; i < num_vert; ++i) {
    double rad = 2 * M_PI * ((i + 1.0) / num_vert);
    double x, y;
    sincos(rad, &y, &x);
    frame[i] = Vector3(float(x * len), 0, float(y * len));
  }
}

void ObjViewer::writeMat(int material_index) {
  if (material_index != -1) {
    obj_material* mat = objDataPtr->materialList[material_index];
    fprintf(f, "usemtl %s\n", mat->name);
  }
}

void ObjViewer::writeFaces() {
  int lastMaterialIndex = 0;
  for (int i = 0; i < objDataPtr->faceCount; ++i) {
    auto face = objDataPtr->faceList[i];
    if (lastMaterialIndex != face->material_index && face->material_index != -1) {
      auto mat = objDataPtr->materialList[face->material_index];
      fprintf(f, "usemtl %s", mat->name);
      lastMaterialIndex = face->material_index;
    }
    fprintf(f, "f");
    for (int j = 0; j < face->vertex_count; ++j) {
      fprintf(f, " %d", face->vertex_index[j] + 1);
      if (face->texture_index[j] != -1)
        fprintf(f, "/%d", face->texture_index[j] + 1);
      if (face->texture_index[j] == -1 && face->normal_index[j] != -1)
        fprintf(f, "/");
      if (face->normal_index[j] != -1)
        fprintf(f, "/%d", face->normal_index[j] + 1);
    }
    fprintf(f, "\n");
  }
  fprintf(f, "\n\n");
}

void ObjViewer::writeSpheres() {
  for (int num_spheres = 0; num_spheres < objDataPtr->sphereCount;
       ++num_spheres) {
    obj_sphere* sphere = objDataPtr->sphereList[num_spheres];
    Vector3 offset = OBJ2GENVEC(objDataPtr->vertexList[sphere->pos_index]);
    Vector3 normal =
        OBJ2GENVEC(objDataPtr->normalList[sphere->up_normal_index]);
    if (normal == Vector3(0, 0, 0))
      normal = Vector3(0, 1, 0);
    else
      normal.normalize();
    Vector3 equatorNorm =
        OBJ2GENVEC(objDataPtr->normalList[sphere->equator_normal_index])
            .normalize();
    if (normal.dot(equatorNorm) != 0)  // ensures equator Norm is orthogonal
      equatorNorm = orthogonalVec(normal).normalize();

    fprintf(f, "o sphere_%d\n", num_spheres);
    writeMat(sphere->material_index);

    // add top vertex
    Vector3 pole_vertex = normal + offset;
    fprintf(f, "v %f %f %f\n", pole_vertex[0], pole_vertex[1], pole_vertex[2]);

    auto w = (normal.cross(equatorNorm)).normalize();
    // generate vertices per stack / slice
    for (int i = 0; i < SPHERE_NUM_STACK - 1; i++) {
      auto phi = M_PI * double(i + 1) / double(SPHERE_NUM_STACK);
      for (int j = 0; j < SPHERE_NUM_SLICES; j++) {
        auto theta = 2.0 * M_PI * double(j) / double(SPHERE_NUM_SLICES);
        double s_phi, c_phi, s_theta, c_theta;
        sincos(phi, &s_phi, &c_phi);
        sincos(theta, &s_theta, &c_theta);
        auto x = float(s_phi * c_theta);
        auto y = float(c_phi);
        auto z = float(s_phi * s_theta);
        Vector3 point = Vector3(x, y, z);
        rotateVec(point, equatorNorm, normal, w);
        point += offset;
        fprintf(f, "v %f %f %f\n", point[0], point[1], point[2]);
      }
    }

    // add bottom vertex
    pole_vertex = -normal + offset;
    fprintf(f, "v %f %f %f\n", pole_vertex[0], pole_vertex[1], pole_vertex[2]);

    //note the - (num_vert + 1) is to use negative indexing
    int num_vert = (SPHERE_NUM_STACK - 1) * SPHERE_NUM_SLICES + 2;
    // add top / bottom triangles
    for (int i = 0; i < SPHERE_NUM_SLICES; ++i) {
      auto i0 = i + 2;
      auto i1 = ((i + 1) % SPHERE_NUM_SLICES) + 2;
      fprintf(f, "f %d %d %d\n", -num_vert, i0 - (num_vert + 1),
              i1 - (num_vert + 1));
      i0 = i + SPHERE_NUM_SLICES * (SPHERE_NUM_STACK - 2) + 2;
      i1 = (i + 1) % SPHERE_NUM_SLICES +
           SPHERE_NUM_SLICES * (SPHERE_NUM_STACK - 2) + 2;
      fprintf(f, "f %d %d %d\n", -1, i0 - (num_vert + 1), i1 - (num_vert + 1));
    }

    // add quads per stack / slice
    for (int j = 0; j < SPHERE_NUM_STACK - 2; j++) {
      auto j0 = j * SPHERE_NUM_SLICES + 1;
      auto j1 = (j + 1) * SPHERE_NUM_SLICES + 1;
      for (int i = 0; i < SPHERE_NUM_SLICES; i++) {
        auto i0 = j0 + i + 1;
        auto i1 = j0 + (i + 1) % SPHERE_NUM_SLICES + 1;
        auto i2 = j1 + (i + 1) % SPHERE_NUM_SLICES + 1;
        auto i3 = j1 + i + 1;
        fprintf(f, "f %d %d %d %d\n", i0 - (num_vert + 1), i1 - (num_vert + 1),
                i2 - (num_vert + 1), i3 - (num_vert + 1));
      }
    }
    fprintf(f, "\n");
  }
  fprintf(f, "\n");
}

void ObjViewer::writePlane() {
  for (int i = 0; i < objDataPtr->planeCount; ++i) {
    obj_plane* plane = objDataPtr->planeList[i];

    fprintf(f, "o plane_%d\n", i);
    writeMat(plane->material_index);

    Vector3 offset = OBJ2GENVEC(objDataPtr->vertexList[plane->pos_index]);
    Vector3 normal = OBJ2GENVEC(objDataPtr->normalList[plane->normal_index]);

    Vector3 v = PLANE_SCALE * orthogonalVec(normal).normalize();
    Vector3 w = PLANE_SCALE * normal.cross(v).normalize();

    Vector3 p1 = v + w + offset;
    Vector3 p2 = -v + w + offset;
    Vector3 p3 = v - w + offset;
    Vector3 p4 = -v - w + offset;
    fprintf(f, "v %f %f %f\n", p1[0], p1[1], p1[2]);
    fprintf(f, "v %f %f %f\n", p2[0], p2[1], p2[2]);
    fprintf(f, "v %f %f %f\n", p3[0], p3[1], p3[2]);
    fprintf(f, "v %f %f %f\n", p4[0], p4[1], p4[2]);
    fprintf(f, "f -1 -2 -4 -3\n");
    writeRay(offset, PLANE_SCALE * normal + offset);
  }
  fprintf(f, "\n");
}

void ObjViewer::writeLightPoints(float scale) {
  const char* pLightString =
      //        "o pointlight\n"
      //        "usemtl LineMaterial2\n"
      //        "v 0 -1 0\n"
      //        "v 1 0 0\n"
      //        "v 0 1 0\n"
      //        "v -1 0 0\n"
      //        "v 0 0 1\n"
      //        "v 0 0 -1\n"
      "l -6 -5 -4 -3 -6\n"
      "l -6 -1 -4 -2 -6\n"
      "l -1 -3 -2 -5 -1\n"
      "f -6 -5 -1\n"
      "f -6 -5 -2\n"
      "f -6 -3 -2\n"
      "f -6 -3 -1\n"
      "f -4 -5 -1\n"
      "f -4 -5 -2\n"
      "f -4 -3 -2\n"
      "f -4 -3 -1\n";
  constexpr size_t num_plight_vert = 6;
  Vector3 pLightFrame[num_plight_vert] = {
      {0, -1, 0}, {1, 0, 0}, {0, 1, 0}, {-1, 0, 0}, {0, 0, 1}, {0, 0, -1},
  };
  for (int i = 0; i < objDataPtr->lightPointCount; ++i) {
    auto pl = objDataPtr->lightPointList[i];
    fprintf(f, "o pointlight_%d\n", i);
    writeMat(pl->material_index);
    Vector3 offset = OBJ2GENVEC(objDataPtr->vertexList[pl->pos_index]);
    //    translateFrame(pLightFrame, num_plight_vert, vec);
    for (auto& vert : pLightFrame) {
      vert *= scale;
      vert += offset;
    }

    for (auto& j : pLightFrame) {
      fprintf(f, "v %f %f %f\n", j[0], j[1], j[2]);
    }
    fprintf(f, "%s\n\n", pLightString);
  }
}

void ObjViewer::writeLightQuad() {
  for (int i = 0; i < objDataPtr->lightQuadCount; ++i) {
    auto quad = objDataPtr->lightQuadList[i];

    fprintf(f, "o quadlight_%d\n", i);
    writeMat(quad->material_index);

    fprintf(f, "f");
    for (int j : quad->vertex_index) {
      fprintf(f, " %d", j + 1);
    }
    fprintf(f, "\n");

    //Note: this is not the true centroid I'm too lazy to find the actual one
    Vector3 p1 = OBJ2GENVEC(objDataPtr->vertexList[quad->vertex_index[0]]);
    Vector3 p2 = OBJ2GENVEC(objDataPtr->vertexList[quad->vertex_index[1]]);
    Vector3 p3 = OBJ2GENVEC(objDataPtr->vertexList[quad->vertex_index[2]]);
    Vector3 midpoint = (p1 + p3) / 2;
    Vector3 normal = (p1 - p2).cross(p2 - p3);
    writeRay(midpoint, normal + midpoint);

    fprintf(f, "\n\n");
  }
}

void ObjViewer::writeLightDisc() {
  constexpr float norm_vec_scale = 3;
  for (int i = 0; i < objDataPtr->lightDiscCount; ++i) {
    auto disc = objDataPtr->lightDiscList[i];
    auto offset = OBJ2GENVEC(objDataPtr->vertexList[disc->pos_index]);
    auto normal = OBJ2GENVEC(objDataPtr->normalList[disc->normal_index]);
    Vector3 discFrame[DISC_LIGHT_NUM_POINTS];
    generateLightDisk(normal, discFrame, DISC_LIGHT_NUM_POINTS);

    // obtain basis vectors for rotation
    // this is jank as hell
    // get a random vector that is not the same as norm
    Vector3 tmp =
        orthogonalVec(normal);  //Vector3(normal[0] + 1, normal[1], normal[2]);
    // cross product the new vector with norm to get u
    Vector3 u = tmp.cross(normal).normalize();
    // cross product again to get w
    Vector3 w = u.cross(normal).normalize();
    // now norm, u, and w should all be orthonormal
    rotateFrame(discFrame, DISC_LIGHT_NUM_POINTS, u, -normal, w);
    for (auto& vert : discFrame)
      vert += offset;

    fprintf(f, "o disclight_%d\n", i);
    writeMat(disc->material_index);

    for (auto& j : discFrame)
      fprintf(f, "v %f %f %f\n", j[0], j[1], j[2]);
    fprintf(f, "f");
    for (int j = 0; j < DISC_LIGHT_NUM_POINTS; ++j)
      fprintf(f, " %d", -(j + 1));
    fprintf(f, "\n");

    //add norm ray
    writeRay(offset, norm_vec_scale * normal + offset);

    fprintf(f, "\n");
  }
}

void ObjViewer::writeCam(float scale) {
  obj_camera* camera = objDataPtr->camera;
  const char* camString =
      //      "o camera\n"
      //      "usemtl LineMaterial2\n"
      //      "v 0 0 0\n"
      //      "v 3 2 4\n"
      //      "v -3 2 4\n"
      //      "v 3 -2 4\n"
      //      "v -3 -2 4\n"
      "l -4 -3 -5 -4 -2 -5 -1 -2\n"
      "l -3 -1\n";
  constexpr size_t num_cam_vert = 5;
  //TODO: match FOV
  Vector3 camFrame[num_cam_vert] = {{0, 0, 0},
                                    //      {5,0,-2},
                                    {3, 2, -4},
                                    {-3, 2, -4},
                                    {3, -2, -4},
                                    {-3, -2, -4}};
  Vector3 offset = OBJ2GENVEC(objDataPtr->vertexList[camera->camera_pos_index]);
  Vector3 lookAtVec =
      OBJ2GENVEC(objDataPtr->vertexList[camera->camera_look_point_index]);
  Vector3 upVec =
      OBJ2GENVEC(objDataPtr->normalList[camera->camera_up_norm_index]);

  // get cam basis vectors
  Vector3 gaze = lookAtVec - offset;
  gaze.normalize();
  Vector3 w = -gaze;
  Vector3 u = upVec.cross(w).normalize();
  Vector3 v = w.cross(u).normalize();

  //  scaleFrame(camFrame, num_cam_vert, scale);
  rotateFrame(camFrame, num_cam_vert, u, v, w);

  //  translateFrame(camFrame, num_cam_vert, offset);
  for (auto& vert : camFrame) {
    vert *= scale;
    vert += offset;
  }

  fprintf(f, "o camera\n");
  for (auto& j : camFrame) {
    fprintf(f, "v %f %f %f\n", j[0], j[1], j[2]);
  }
  fprintf(f, "%s", camString);

  // add look at vector
//  writeRay(lookAtVec, offset);

  // add up vector
  Vector3 camNorm =
      OBJ2GENVEC(objDataPtr->normalList[camera->camera_up_norm_index]);
  Vector3 camNormOffset = 2 * scale * camNorm + offset;
  writeRay(camNormOffset, offset);

  fprintf(f, "\n");
}

void ObjViewer::writeRay(Vector3 startPoint, Vector3 endPoint) {
  fprintf(f, "v %f %f %f\n", startPoint[0], startPoint[1], startPoint[2]);
  fprintf(f, "v %f %f %f\n", endPoint[0], endPoint[1], endPoint[2]);
  fprintf(f, "l -1 -2\n");

  // == drawing arrow head ==
  // the arrow head is a pyramid so four new points need to be created
  Vector3 reversedVec = startPoint - endPoint;
  float len = reversedVec.length();
  Vector3 scaledVector = 0.08f * reversedVec;
  const Vector3& basisVec1 =
      0.03f * len * orthogonalVec(scaledVector).normalize();
  const Vector3& basisVec2 =
      0.03f * len * scaledVector.cross(basisVec1).normalize();

  // getting the 4 new points for the arrow head
  Vector3 offset = endPoint + scaledVector;
  Vector3 newPoint1 = offset + basisVec1;
  Vector3 newPoint2 = offset + basisVec2;
  Vector3 newPoint3 = offset - basisVec1;
  Vector3 newPoint4 = offset - basisVec2;

  fprintf(f, "v %f %f %f\n", newPoint1[0], newPoint1[1], newPoint1[2]);
  fprintf(f, "v %f %f %f\n", newPoint2[0], newPoint2[1], newPoint2[2]);
  fprintf(f, "v %f %f %f\n", newPoint3[0], newPoint3[1], newPoint3[2]);
  fprintf(f, "v %f %f %f\n", newPoint4[0], newPoint4[1], newPoint4[2]);

  fprintf(f, "f -5 -1 -2\n");
  fprintf(f, "f -5 -2 -3\n");
  fprintf(f, "f -5 -3 -4\n");
  fprintf(f, "f -5 -4 -1\n");

  fprintf(f, "f -1 -2 -3 -4\n");  // back face of the arrow head
}

void ObjViewer::editMtl(const char* inputFile, const char* outputFile) {
  /*
  std::string line;
  std::ifstream in_file(inputFile);
  std::ofstream out_file(outputFile);
  if (in_file && out_file) {
    //TODO: replace the r in .mtl files to be commented
    while (getline(in_file, line)) {
      if (line[0] == 'r')
        line.insert(0, "#");
      out_file << line << "\n";
    }
  } else {
    // Something went wrong
    printf("Cannot read File");
  }
  // Closing file
  in_file.close();
  out_file.close();
   */
}

void ObjViewer::init(const objLoader& objData, const char* outputFile,
                     const char* outputMatFile) {
  initialized = true;
  fileName = outputFile;
  f = fopen(outputFile, "w");
  if (f == nullptr) {
    printf("Error opening file %s\n", outputFile);
    exit(1);
  }
  objDataPtr = &objData;
  //  editMtl(outputMatFile, outputMatFile);

  //TODO: add comments into the output file

  // adding material file
  fprintf(f, "mtllib %s\n\n", outputMatFile);

  // -- writing vectors --
  for (int i = 0; i < objData.vertexCount; ++i) {
    auto vec = objData.vertexList[i]->e;
    fprintf(f, "v %f %f %f\n", vec[0], vec[1], vec[2]);
  }
  fprintf(f, "\n");

  // -- writing normals --
  for (int i = 0; i < objData.normalCount; ++i) {
    auto vec = objData.normalList[i]->e;
    fprintf(f, "vn %f %f %f\n", vec[0], vec[1], vec[2]);
  }
  fprintf(f, "\n");

  // -- writing textures --
  for (int i = 0; i < objData.textureCount; ++i) {
    auto vec = objData.textureList[i]->e;
    fprintf(f, "vt %f %f %f\n", vec[0], vec[1], vec[2]);
  }
  fprintf(f, "\n");

  // -- writing objects --
  writeFaces();
  writePlane();
  writeLightPoints();
  writeLightQuad();
  writeLightDisc();
  writeCam();
  writeSpheres();

  // -- Closing file --
  fclose(f);
}

void ObjViewer::addRay(const Vector3& origin, const Vector3& direction,
                       float t) {
  if (!initialized) {
    perror(
        "error: ObjDebugger not initialized.\n\tObjDebugger needs to parse "
        "the obj file\n");
    throw std::runtime_error(
        "Could not find file. Did you call ObjViewer::get().init()");
  }
  f = fopen(fileName, "a");
  if (f == nullptr) {
    printf("Error opening file %s\n", fileName);
    exit(1);
  }
  Vector3 p = origin + t * direction;

  fprintf(f, "o ray\n");
  writeRay(origin, p);
  fclose(f);
  f = nullptr;
}

#endif
