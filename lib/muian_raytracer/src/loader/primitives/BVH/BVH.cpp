//
// Created by agmui on 1/27/25.
//

#include "BVH.h"

AABB enclose(const std::vector<std::shared_ptr<Primitive>>& primList, int lower,
             int upper) {
  //FIXME: handel case of upper == lower

  const auto& obj = primList[lower];
  float tmp[3];
  obj->getMax(tmp);
  float curMax[3] = {tmp[0], tmp[1], tmp[2]};
  obj->getMin(tmp);
  float curMin[3] = {tmp[0], tmp[1], tmp[2]};
  for (int i = lower + 1; i <= upper; ++i) {
    const auto& obj = primList[i];
    float outPos[3];

    obj->getMax(outPos);
    curMax[0] = std::max(curMax[0], outPos[0]);
    curMax[1] = std::max(curMax[1], outPos[1]);
    curMax[2] = std::max(curMax[2], outPos[2]);

    obj->getMin(outPos);
    curMin[0] = std::min(curMin[0], outPos[0]);
    curMin[1] = std::min(curMin[1], outPos[1]);
    curMin[2] = std::min(curMin[2], outPos[2]);
  }
  return {curMin[0], curMin[1], curMin[2], curMax[0], curMax[1], curMax[2]};
}

BVHnode BVH::helper(std::vector<std::shared_ptr<Primitive>>& primList,
                    AABB& aabb, Splitter& splitter, int lower, int upper,
                    int depth) {

  if (depth >= MAX_BVH_DEPTH) {
    printf("===lim\n");
//    BVHnode ret(lower, upper, aabb, false, false, true);
//    return ret;
  }

  int bound = splitter.split(primList, aabb, lower, upper);

  bool lLeaf = false;
  bool rLeaf = false;
  if (bound - 1 <= lower) {  //leaf left case
    lLeaf = true;
  }
  if (upper <= bound) {  // leaf right case
    rLeaf = true;
  }
  BVHnode lnode;
  BVHnode rnode;
  if (!lLeaf) {
    AABB aabbL = enclose(primList, lower, bound - 1);
    lnode = helper(primList, aabbL, splitter, lower, bound - 1, depth + 1);
  }
  if (!rLeaf) {
    AABB aabbR = enclose(primList, bound, upper);
    rnode = helper(primList, aabbR, splitter, bound, upper, depth + 1);
  }
  if (!lLeaf)
    nodeArr.push_back(lnode);
  else{
    AABB a_tmp(0,0,0,0,0,0);
    BVHnode l_tmp(lower, a_tmp, true);
    nodeArr.push_back(l_tmp);
  }
  if (!rLeaf)
    nodeArr.push_back(rnode);
  else{
    AABB a_tmp(0,0,0,0,0,0);
    BVHnode r_tmp(bound, a_tmp, true);
    nodeArr.push_back(r_tmp);
  }

//  return {(lLeaf) ? lower : static_cast<int>(nodeArr.size() - (1 + !rLeaf)),
//          (rLeaf) ? bound : static_cast<int>(nodeArr.size() - 1), aabb, lLeaf,
//          rLeaf};

  return {static_cast<uint>(nodeArr.size() - 2),
//          static_cast<int>(nodeArr.size() - 1),
          aabb,
          false};
}

void BVH::buildTree(const Camera& camera, const std::shared_ptr<Scene>& scene) {
  nodeArr.reserve(scene->getPrimitiveList().size());  //TODO: optimize

  int lower = 0;
  int upper = scene->getPrimitiveList().size() - 1;
  AABB aabb = enclose(scene->getPrimitiveList(), lower, upper);
  BVHnode root =
      helper(scene->getPrimitiveList(), aabb, splitter, lower, upper, 0);
  nodeArr.push_back(root);
  rootIndex = nodeArr.size() - 1;
}

///*
hitpoint BVH::hitIterative(const Ray& r, float cutoffBound) {
  float og_cutoffBound = cutoffBound;
  const auto& primList = scene->getPrimitiveList();
  uint stack[MAX_BVH_DEPTH];
  int stackIndex = 0;
  stack[stackIndex] = rootIndex;
  stackIndex++;
  hitpoint best_hp = {nullptr, VEC_0, cutoffBound};
  while (stackIndex > 0) {
    stackIndex--;
    BVHnode curNode = nodeArr[stack[stackIndex]];
    hitpoint hp = curNode.hit(r, cutoffBound);
    if (hp.t != -1) {
      if (curNode._isList())
        printf("ERROR BVH tree to big implement prim list\n");
      if (nodeArr[curNode.getNodeL()].is_leaf()) {
        const auto& prim = primList[nodeArr[curNode.getNodeL()].getNodeL()];
        const hitpoint& prim_hp = prim->hit(r, cutoffBound);
        if(prim_hp.t != -1 && prim_hp.t < best_hp.t){
          best_hp = prim_hp;
          cutoffBound = best_hp.t;
        }
      } else {
        stack[stackIndex] = curNode.getNodeL();
        stackIndex++;
      }
      if (nodeArr[curNode.getNodeR()].is_leaf()) {
        const auto& prim = primList[nodeArr[curNode.getNodeR()].getNodeL()];
        const hitpoint& prim_hp = prim->hit(r, cutoffBound);
        if(prim_hp.t != -1 && prim_hp.t < best_hp.t){
          best_hp = prim_hp;
          cutoffBound = best_hp.t;
        }
      } else {
        stack[stackIndex] = curNode.getNodeR();
        stackIndex++;
      }
    }
  }
  if (best_hp.t >= og_cutoffBound) {
    return {nullptr, VEC_0, -1};
  } else {
    return best_hp;
  }
}
// */

//TODO: just pass in hitpoint
hitpoint BVH::hitRecurse(BVHnode& node, const Ray& r, float cutoffBound) {
    if (node.is_leaf()) {
      const hitpoint& hp2 =
          scene->getPrimitiveList()[node.getNodeL()]->hit(r, cutoffBound);
      return hp2;
    }
  const hitpoint& hp = node.hit(r, cutoffBound);
  if (hp.t != -1) {
    //    if (node._isList()) {
    ////      printf("in hit recurse: %d\n", node.getNodeL());
    //      auto primArr = scene->getPrimitiveList();
    //      hitpoint best_hp = primArr[node.getNodeL()]->hit(r, cutoffBound);
    //      //TODO: check if best_hp is not -1
    //      for (int i = node.getNodeL() + 1; i <= node.getNodeR(); ++i) {
    //        const hitpoint& hp2 = primArr[i]->hit(r, best_hp.t);
    //        if (hp2.t != -1)
    //          best_hp = hp2.t < best_hp.t ? hp2 : best_hp;
    //      }
    ////      printf("is list: %f\n", best_hp.t);
    //      return best_hp;
    //    }

    //TODO: find a way to check the closer box first by moving the hit call up
    // const Vector3& a = nodeArr[node.getNodeL()].getCentroid();
    // const Vector3& b = nodeArr[node.getNodeR()].getCentroid();
    // float len_a = (a - (r.getOrigin() + r.getDirection())).squaredLength();
    // float len_b = (b - (r.getOrigin() + r.getDirection())).squaredLength();
    // if(len_a < len_b){
    //
    // }else{
    //
    // }


    const hitpoint& l_rez =
//        (nodeArr[node.getNodeL()].is_leaf())
//            ? scene->getPrimitiveList()[nodeArr[node.getNodeL()].getNodeL()]->hit(r, cutoffBound)
//            : hitRecurse(nodeArr[node.getNodeL()], r, cutoffBound);
    hitRecurse(nodeArr[node.getNodeL()], r, cutoffBound);
    cutoffBound =
        (l_rez.t != -1 && l_rez.t < cutoffBound) ? l_rez.t : cutoffBound;
    const hitpoint& r_rez =
//        (nodeArr[node.getNodeR()].is_leaf())
//            ? scene->getPrimitiveList()[nodeArr[node.getNodeR()].getNodeL()]->hit(r, cutoffBound)
//            : hitRecurse(nodeArr[node.getNodeR()], r, cutoffBound);
    hitRecurse(nodeArr[node.getNodeR()], r, cutoffBound);

    int c1 = l_rez.t != -1 && l_rez.t < cutoffBound;
    int c2 = (r_rez.t != -1 && r_rez.t < cutoffBound) << 1;
    hitpoint hp_arr[4] = {l_rez, l_rez, r_rez,
                          l_rez.t < r_rez.t ? l_rez : r_rez};
    return hp_arr[c2 + c1];
  }
  return {nullptr, VEC_0, -1};
}

hitpoint BVH::hit(const Ray& r, float cutoffBound) {
  auto root = nodeArr[rootIndex];
  hitpoint hp;
//  return hitRecurse(root, r, cutoffBound);
  return hitIterative(r, cutoffBound);
}
void BVH::printDotfile() {
      /*
  printf("digraph G{\n");
  for (int i = 0; i < nodeArr.size(); ++i) {
    if (!nodeArr[i].Lisleaf()) {
      printf("%d -> %d\n", i, nodeArr[i].getNodeL());
    } else {
      printf("%d -> leaf_%d\n", i, nodeArr[i].getNodeL());
    }
    if (!nodeArr[i].Risleaf()) {
      printf("%d -> %d\n", i, nodeArr[i].getNodeR());
    } else {
      printf("%d -> leaf_%d\n", i, nodeArr[i].getNodeR());
    }
  }
  printf("}\n");
  //   */
}
