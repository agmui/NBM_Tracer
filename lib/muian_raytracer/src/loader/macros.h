//
// Created by agmui on 12/10/24.
//

#ifndef TRACER_MACROS_H
#define TRACER_MACROS_H

//#include <math.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <stdint.h>
//#include <memory>
//#include <iostream>
//#include <iomanip>
//#include <sstream>
//#include <chrono>
//#include <ctime>
//#include <cmath>


#include <GenVector.h>
#include <objLoader.hpp>

#define MAX_BOUNCES 4
#define MAX_BVH_DEPTH 50 //40

//#define TRANSPARENCY
#define MAX_RAY_DIST 10000000.0f
#define VALID_HITPOINT (-0.0f)

// ====jason stuff====
/*
#define LOG_RESET "\033[0m"
#define LOG_RED "\033[31m"
#define LOG_BLUE "\033[34m"
#define LOG_GREEN "\033[32m"
#define LOG_YELLOW "\033[33m"
#define LOG_PURPLE "\033[35m"
#define LOG_CYAN "\033[36m"
#define LOG_INFO(...)  {printf("%s[INFO]%s  ", LOG_GREEN, LOG_RESET);  printf(VA_ARGS); printf("\n");}
#define LOG_FATAL(...) {printf("%s[FATAL]%s ", LOG_RED, LOG_RESET);    printf(VA_ARGS); printf("\n"); exit(1);}
#define LOG_WARN(...)  {printf("%s[WARN]%s  ", LOG_YELLOW, LOG_RESET); printf(VA_ARGS); printf("\n");}
#define LOG_DEBUG(...) {if(Debugging::DebuggingEnabled()){printf("%s[DEBUG]%s ", LOG_BLUE, LOG_RESET);   printf(VA_ARGS); printf("\n");}}
#define LOG_CUSTOM(precursor, ...) {printf("%s[%s]%s  ", LOG_CYAN, precursor, LOG_RESET);   printf(VA_ARGS); printf("\n");}
#define LOG_SCAN(...)  {printf("%s[INPUT]%s ", LOG_PURPLE, LOG_RESET); scanf(VA_ARGS);}
#define LOG_ASSERT(x, ...) if (!(x)) { printf("%s[FAIL]%s  Assertion failed in %s:%d - "", LOG_RED, LOG_RESET, FILE, LINE); printf(VA_ARGS); printf(""\n"); exit(0); }
#define START_PROFILE(id) uint64_t id = TIMENOW();
#define END_PROFILE(id, x) { \
    id = TIMENOW() - id; \
    std::stringstream profiler_logstream; \
    profiler_logstream << x \
        << std::setw(2) << std::setfill('0') << (id / 3600000) << ':' \
        << std::setw(2) << std::setfill('0') << ((id / 60000) % 60) << ':' \
        << std::setw(2) << std::setfill('0') << ((id / 1000) % 60) << ':' \
        << std::setw(3) << std::setfill('0') << (id % 1000); \
    LOG_INFO("%s", profiler_logstream.str().c_str()); \
}
// */
#define TIMENOW() (uint64_t)(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch())).count()
//==================

//TODO: make these all refer to the same obj in usage
#define WHITE Color(255, 255, 255)
#define BLACK Color(0, 0, 0)
#define RED Color(255, 0, 0)
#define GREEN Color(0, 255, 0)
#define BLUE Color(0, 0, 255)

#define VEC_0 Vector3()
//Vector3* zero = new Vector3 (0,0,0);

//This might be helpful to convert from obj vectors to GenVectors
#define OBJ2GENVEC(objVec) \
  Vector3((float)objVec->e[0], (float)objVec->e[1], (float)objVec->e[2])

// converts from obj_material to Material
#define OBJ2MAT(matData)                                                  \
  std::make_shared<Material>(                                             \
      matData->name, matData->texture_filename,                           \
      Vector3((float)matData->amb[0], (float)matData->amb[1],             \
              (float)matData->amb[2]),                                    \
      Vector3((float)matData->diff[0], (float)matData->diff[1],           \
              (float)matData->diff[2]),                                   \
      Vector3((float)matData->spec[0], (float)matData->spec[1],           \
              (float)matData->spec[2]),                                   \
      matData->reflect, matData->refract, matData->trans, matData->shiny, \
      matData->glossy, matData->refract_index)

#define VEC2COLOR(v)                                      \
  Color(static_cast<unsigned char>(abs((v)[0] * 255.0f)), \
        static_cast<unsigned char>(abs((v)[1] * 255.0f)), \
        static_cast<unsigned char>(abs((v)[2] * 255.0f)))

inline Vector3 rotX(const Vector3& v, float phi) {
  return {v[0], cos(phi) * v[1] - sin(phi) * v[2],
          sin(phi) * v[1] + cos(phi) * v[2]};
}

// TODO: use sincos
//inline Vector3 rotX(const Vector3& v, float phi) {
//  float c, s;
//  sincos(phi, reinterpret_cast<double*>(&c), reinterpret_cast<double*>(&s));
//  return {v[0], c * v[1] - s * v[2], s * v[1] + c * v[2]};
//}

inline Vector3 rotY(const Vector3& v, float theta) {
  return {cos(theta) * v[0] + sin(theta) * v[2], v[1],
          -sin(theta) * v[0] + cos(theta) * v[2]};
}
//inline Vector3 rotY(const Vector3& v, float theta) {
//  float c, s;
//  sincos(theta, reinterpret_cast<double*>(&c), reinterpret_cast<double*>(&s));
//  return {c * v[0] + s * v[2], v[1], -s * v[0] + c * v[2]};
//  //  return {cos(theta) * v[0] + sin(theta) * v[2], v[1],
//  //          -sin(theta) * v[0] + cos(theta) * v[2]};
//}
inline Vector3 rotZ(const Vector3& v, float psi) {
  //  float c, s;
  //  sincos(psi, reinterpret_cast<double*>(&c), reinterpret_cast<double*>(&s));
  //  return {c * v[0] - s * v[1], s * v[0] + c * v[1], v[2]};
  return {cos(psi) * v[0] - sin(psi) * v[1], sin(psi) * v[0] + cos(psi) * v[1],
          v[2]};
}
//TODO:
//Vector3 Vector3::norm() const{
//
//}

//TODO: add into gen vec class
inline float atan2_angle(float v1, float v2, float w1, float w2) {
  return atan2(w2 * v1 - w1 * v2, w1 * v1 + w2 * v2);
}

#ifdef OBJ_DEBUG
//extern int cord[2];
extern bool draw;
#endif  //OBJ_DEBUG

#endif  //TRACER_MACROS_H
