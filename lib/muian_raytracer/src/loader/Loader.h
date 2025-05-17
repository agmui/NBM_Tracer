//
// Created by agmui on 12/5/24.
//

#ifndef TRACER_LOADER_H
#define TRACER_LOADER_H

#include <cassert>
#include <iostream>
#include <memory>

#include <objLoader.hpp>

#include "Camera.h"
#include "Lights/Light.h"
#include "Lights/PointLight.h"
#include "Material.h"
#include "Scene.h"
#include "macros.h"
#include "primitives/shapes/Sphere.h"
#include "src/RayTracer.h"

//TODO: make interface for stl or other files
class Loader {
 public:
  explicit Loader(char* filename) {
    //load obj from file filename
    objData.load(filename);
#ifdef OBJ_DEBUG
    std::ifstream file(filename);
    std::string line;
    while (getline(file, line) ) {
//      printf("%s\n", line.c_str());
      if (line[0] != '#' && line != "\n" && strncmp(line.c_str(), "mtllib ", 7) == 0)
        break;
    }
    ObjViewer::get().init(
        objData, "debug.obj",
//        line.substr(7, line.length()).c_str()
        //                                  "./cornell_box.mtl"
                                  "./resources/scenes/cornell_box.mtl"
        //                        "./resources/scenes/blue_sphere.mtl"
        //                                  "./resources/part1/test.mtl"
    );
#endif
  }
  std::shared_ptr<Scene> load(char* filename) const;
  Camera loadCamera() const;

 private:
  objLoader objData;
};

#endif  //TRACER_LOADER_H
