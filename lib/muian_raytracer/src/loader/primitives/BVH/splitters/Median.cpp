//
// Created by agmui on 1/27/25.
//

#include "Median.h"

float splitPrimList(const float xyz[3], int largest, const AABB& aabb,
            std::vector<std::shared_ptr<Primitive>>& primList, int& i, int& j) {

  float divLine = xyz[largest] / 2 + aabb.getBbMin()[largest];

//  while (i <= j) {
//    bool leftCheck = primList[i]->getCentroid()[largest] >= divLine;
//    bool rightCheck = primList[j]->getCentroid()[largest] < divLine;
//    if (leftCheck && rightCheck)
//      std::swap(primList[i], primList[j]);
//    i += !leftCheck | rightCheck;
//    j -= leftCheck | !rightCheck;
//  }
  while(i <= j){
    float center = primList[i]->getCentroid()[largest];
    if(center <= divLine){
      i++;
    }else{
      std::swap(primList[i], primList[j--]);//TODO: do indirection but then resave the indirection to not need the 2 array cost
    }
  }
  return divLine;
}

int Median::split(std::vector<std::shared_ptr<Primitive>>& primList, AABB& aabb,
                  int lower, int upper) {

  float x = aabb.getBbMax()[0] - aabb.getBbMin()[0];
  float y = aabb.getBbMax()[1] - aabb.getBbMin()[1];
  float z = aabb.getBbMax()[2] - aabb.getBbMin()[2];

  int best_axis_index = 0;
  float biggest = std::abs(x);
  if (biggest < std::abs(y)) {
    biggest = std::abs(y);
    best_axis_index = 1;
  }
  if (biggest < std::abs(z)) {
    biggest = std::abs(z);  //TODO: remove?
    best_axis_index = 2;
  }

  //manual loop unroll
  int i = lower, j = upper;
  float xyz[3] = {x, y, z};
  float divLine = splitPrimList(xyz, best_axis_index, aabb, primList, i, j);
  if (i > upper || j < lower) {
    i = lower;
    j = upper;
    divLine = splitPrimList(xyz, (best_axis_index + 1) % 3, aabb, primList, i, j);
    if (i > upper || j < lower) {
      i = lower;
      j = upper;
      divLine = splitPrimList(xyz, (best_axis_index + 2) % 3, aabb, primList, i, j);
    }
  }

  /*
  float xyz[3] = {x, y, z};
  int i, j;
  for (int itter = 0; itter < 3; ++itter) {
    float divLine = xyz[largest] / 2 + aabb.getBbMin()[largest];

    i = lower;
    j = upper;
    while (i <= j) {
      bool leftCheck = primList[i]->getCentroid()[largest] >= divLine;
      bool rightCheck = primList[j]->getCentroid()[largest] < divLine;
      if (leftCheck && rightCheck)
        std::swap(primList[i], primList[j]);
      i += !(leftCheck & !rightCheck);
      j -= !(!leftCheck & rightCheck);
    }

    if (i > upper || j < lower) {
      largest = (largest + 1) % 3;
    } else {
      break;
    }
  }
   */

//  info->divLine = divLine;
//  info->axis = best_axis_index;
  if (i > upper || j < lower)
    return (lower + upper) / 2 + 1;
  return i;
}
