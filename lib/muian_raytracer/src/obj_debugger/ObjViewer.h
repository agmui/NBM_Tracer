//
// Created by agmui on 12/22/24.
//

#ifndef TRACER_OBJVIEWER_H
#define TRACER_OBJVIEWER_H

#ifdef OBJ_DEBUG

#include <cmath>
#include <cstdio>
#include <fstream> //TODO: remove the throw err

#include <GenVector.h>
#include <objLoader.hpp>

#define DISC_LIGHT_NUM_POINTS 12  /// number of point a light disc will have
#define SPHERE_NUM_STACK 8        /// number of stacks a sphere will have
#define SPHERE_NUM_SLICES 8       /// number of slices a sphere will have
#define PLANE_SCALE 100.0f        /// scale for how big the planes are
#define LIGHTPOINT_SCALE 1       /// scale for how big the light points are
#define CAMERA_SCALE 2            /// scale for how big the camera is

/// Might be helpful to convert from obj_vectors* to GenVectors
#define OBJ2GENVEC(objVec) \
  Vector3((float)objVec->e[0], (float)objVec->e[1], (float)objVec->e[2])

/**
 * @brief Debugging class that takes objLoader and
 * makes an .obj file that can be viewed in any model viewer
 *
 * NOTE: ObjViewer is a SINGLETON\n
 *
 * This class uses a special .obj loader:
 * <a href="https://kixor.net/dev/objloader/">Wavefront (obj) loader</a>
 * that breaks the .obj spec
 *
 * @code
 *    objData.load("./cornell_box.obj");
 *    ObjViewer::get().init(objData, "debug.obj", "./cornell_box.mtl");
 *    Ray r = ...;
 *    hitpoint hp = ...;
 *    ObjViewer::get().addRay(r.origin, r.direction, hp.t);
 * @endcode
 */
class ObjViewer {
 public:
  /**
   * Gets a reference to the ObjViewer singleton
   *
   * @code
   *    ObjViewer::get()
   * @endcode
   *
   * @return ObjViewer&
   */
  static ObjViewer& get() {
    static ObjViewer objDebugger;
    return objDebugger;
  }

 private:
  ObjViewer() {}

  static bool initialized;        /// bool to check if init() has been called
  const char* fileName{};         /// output .obj filename
  FILE* f = nullptr;              /// .obj file handel
  const objLoader* objDataPtr{};  /// objLoader data

  void writeMat(int material_index);
  void writeFaces();
  /**
   * Sphere code is taken from:
   * https://danielsieger.com/blog/2021/03/27/generating-spheres.html
   */
  void writeSpheres();
  void writePlane();
  /**
   * @param scale scales the light point model
   */
  void writeLightPoints(float scale = LIGHTPOINT_SCALE);
  void writeLightQuad();
  void writeLightDisc();
  /**
   * Draws a camera wireframe with the up and gaze vectors
   *
   * @param scale scales the camera model
   */
  void writeCam(float scale = CAMERA_SCALE);
  void writeRay(Vector3 startPoint, Vector3 endPoint);
  void editMtl(const char* inputFile, const char* outputFile);

 public:
  ObjViewer(ObjViewer const&) = delete;
  void operator=(ObjViewer const&) = delete;

  /**
   * @brief Initializes the ObjDebugger class
   *
   * Note:\n
   * you need to give the path of the .mtl file objData uses
   *
   * @code
   *    objLoader objData;
   *    objData.load("./cornell_box.obj");
   *    ObjViewer::get().init(objData, "debug.obj", "./cornell_box.mtl");
   * @endcode
   *
   * @param objData         data from the input .obj file
   * @param outputFile      name of the output .obj
   * @param outputMatFile   path to the .mtl file objData uses
   */
  void init(const objLoader& objData, const char* outputFile,
            const char* outputMatFile);

  /**
   * @brief Adds a ray to the output .obj file\n
   *
   * NOTE:\n
   *    ObjViewer::get().init() must be called first
   *
   * @code
   *    objData.load("./cornell_box.obj");
   *    ObjViewer::get().init(objData, "debug.obj", "./cornell_box.mtl");
   *    Ray r = ...;
   *    hitpoint hp = ...;
   *    ObjViewer::get().addRay(r.origin, r.direction, hp.t);
   * @endcode
   *
   * @param origin          Ray's origin
   * @param direction       Ray's direction vector
   * @param t               Ray's hit point
   *
   * @throws runtime_error
   */
  void addRay(const Vector3& origin, const Vector3& direction, float t);
};
#endif
#endif  //TRACER_OBJVIEWER_H
