//
// Created by agmui on 2/9/25.
//

#include "SAH.h"

float _splitPrimList(const float xyz[3], int largest, const AABB& aabb,
                    std::vector<std::shared_ptr<Primitive>>& primList, int& i,
                    int& j) {

  float divLine = xyz[largest] / 2 + aabb.getBbMin()[largest];

  while (i <= j) {
    float center = primList[i]->getCentroid()[largest];
    if (center <= divLine) {
      i++;
    } else {
      std::swap(
          primList[i],
          primList
              [j--]);  //TODO: do indirection but then resave the indirection to not need the 2 array cost
    }
  }
  return divLine;
}

int SAH::split(std::vector<std::shared_ptr<Primitive>>& primList, AABB& aabb,
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
  int i_0 = lower, j_0 = upper;
  float xyz[3] = {x, y, z};
  _splitPrimList(xyz, best_axis_index, aabb, primList, i_0, j_0);
  int i_1 = lower, j_1 = upper;
  _splitPrimList(xyz, (best_axis_index + 1) % 3, aabb, primList, i_1, j_1);
  int i_2 = lower, j_2 = upper;
  _splitPrimList(xyz, (best_axis_index + 2) % 3, aabb, primList, i_2, j_2);

  int midpoint = (lower+upper)/2+1;
  int dist_0 = std::abs(midpoint - i_0);
  int dist_1 = std::abs(midpoint - i_1);
  int dist_2 = std::abs(midpoint - i_2);
  if ((i_0 > upper || j_0 < lower) && (i_1 > upper || j_1 < lower) && (i_2 > upper || j_2 < lower))
    return midpoint;

  if(dist_0 < dist_1 ){
   if(dist_0 < dist_2 ){
     int i_0 = lower, j_0 = upper;
     _splitPrimList(xyz, best_axis_index, aabb, primList, i_0, j_0);
     return i_0;
   } else{
     int i_2 = lower, j_2 = upper;
     _splitPrimList(xyz, (best_axis_index + 2) % 3, aabb, primList, i_2, j_2);
     return i_2;
   }
  }else{
    if(dist_1 < dist_2){
      int i_1 = lower, j_1 = upper;
      _splitPrimList(xyz, (best_axis_index + 1) % 3, aabb, primList, i_1, j_1);
      return i_1;
    } else{
      int i_2 = lower, j_2 = upper;
      _splitPrimList(xyz, (best_axis_index + 2) % 3, aabb, primList, i_2, j_2);
      return i_2;
    }
  }

//  if (i > upper || j < lower)
//    return (lower + upper) / 2 + 1;
//  return i;
}
